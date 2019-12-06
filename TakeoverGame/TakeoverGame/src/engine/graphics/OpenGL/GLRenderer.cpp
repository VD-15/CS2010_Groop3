#include "GLRenderer.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

#include "utf8/utf8.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <set>

#include "../../components/DrawPrimitiveComponent.h"
#include "../../components/DrawTextureComponent.h"
#include "../../components/DrawStringComponent.h"
#include "../../components/DrawModelComponent.h"
#include "../../components/CameraComponent.h"
#include "../../components/LightComponent.h"
#include "../../memory/ChunkAllocator.hpp"
#include "../../utils/StringCollection.h"
#include "../../utils/ByteBuffer.hpp"
#include "../../utils/Matrix.h"
#include "../../core/Window.h"
#include "../../core/Game.h"
#include "../../core/ContentManager.hpp"
#include "../../utils/VLKIO.h"

#include "VertexArray.h"

using namespace vlk;

namespace
{
	Matrix4 viewport2D;
	Matrix4 viewport3D;

	//Maps textures to their handles
	std::map<const Texture2D*, UInt> textureMap;

	//Maps models to mesh buffers
	std::map<const Model*, std::vector<GLBuffer>> modelMap;

	//Texture2DVAO texture2Dvao;
	ModelVAO* modelVAO;

	constexpr UInt MAX_POINT_LIGHTS = 16;
	constexpr UInt MAX_DRCTN_LIGHTS = 4;

	void OnFramebufferResize(vlk::WindowFramebufferEvent& ev)
	{
		glViewport(0, 0, ev.width, ev.height);
	}

	void OnModelLoad(vlk::ContentLoadedEvent<Model>& ev)
	{
		const Model* model = ev.content;

		ULong meshCount = model->GetMeshes().size();

		modelMap[model] = std::vector<GLBuffer>();
		std::vector<GLBuffer>& buffers = modelMap[model];
		buffers.reserve(meshCount);

		for (ULong i = 0; i < meshCount; i++)
		{
			const std::vector<UInt>& mesh = model->GetMeshes()[i];

			ByteBuffer b;
			b.Allocate((mesh.size() / 3) * (3 * 2 * 3) * 4);

			//Assemble mesh data into perVertexBuffer
			for (ULong j = 0; j < mesh.size(); j += 3)
			{
				ULong vertex = static_cast<ULong>(mesh[j + 0] - 1) * 3;
				ULong coord = static_cast<ULong>(mesh[j + 1] - 1) * 2;
				ULong norm = static_cast<ULong>(mesh[j + 2] - 1) * 3;

				b.Put<Float>(model->GetVertices()[vertex + 0]);
				b.Put<Float>(model->GetVertices()[vertex + 1]);
				b.Put<Float>(model->GetVertices()[vertex + 2]);

				b.Put<Float>(model->GetCoords()[coord + 0]);
				b.Put<Float>(model->GetCoords()[coord + 1]);

				b.Put<Float>(model->GetNormals()[norm + 0]);
				b.Put<Float>(model->GetNormals()[norm + 1]);
				b.Put<Float>(model->GetNormals()[norm + 2]);
			}

			buffers.emplace_back();
			GLBuffer& g = buffers[i];
			g.Bind(GL_ARRAY_BUFFER);
			g.Resize(b.Size(), GL_STATIC_COPY, b.Data());
			g.Unbind();
		}
	}

	void OnModelUnload(vlk::ContentUnloadedEvent<Model>& ev)
	{
		modelMap.erase(ev.content);
	}

	void OnTextureLoad(vlk::ContentLoadedEvent<Texture2D>& ev)
	{
		const std::string& wrapMode = ev.content->GetMetaTag("wrap_mode");
		const std::string& filterMode = ev.content->GetMetaTag("filter_mode");
		const std::string& genMipmaps = ev.content->GetMetaTag("mipmap");

		Int wrapEnum = GL_CLAMP_TO_BORDER;
		Int filterEnumMin = GL_LINEAR;
		Int filterEnumMax = GL_LINEAR;
		Int genMipMapEnum = 1;

		if (wrapMode == "repeat")		wrapEnum = GL_REPEAT;
		else if (wrapMode == "mirror")	wrapEnum = GL_MIRRORED_REPEAT;
		else if (wrapMode == "clamp")	wrapEnum = GL_CLAMP_TO_EDGE;

		if (genMipmaps == "false")
		{
			genMipMapEnum = 0;

			if (filterMode == "nearest") 
			{ 
				filterEnumMin = GL_NEAREST; 
				filterEnumMax = GL_NEAREST; 
			}
		}
		else
		{
			filterEnumMax = GL_LINEAR;
			filterEnumMin = GL_LINEAR_MIPMAP_LINEAR;
		}

		//Create texture object
		UInt texture;
		glGenTextures(1, &texture);

		//Bind and load texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ev.content->Width(), ev.content->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ev.content->Data());

		//Set wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapEnum);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapEnum);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterEnumMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterEnumMax);

		//Generate mipmaps if specified.
		if (genMipMapEnum) glGenerateMipmap(GL_TEXTURE_2D);

		//Add texture handle to texture map
		textureMap[ev.content] = texture;

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OnTextureUnload(vlk::ContentUnloadedEvent<Texture2D>& ev)
	{
		UInt texture = textureMap[ev.content];

		glDeleteTextures(1, &texture);
	}

	void DrawModels3D()
	{
		modelVAO->Bind();

		std::vector<const DrawModelComponent3D*> allModels;
		allModels.reserve(DrawModelComponent3D::GetCount());
		
		{
			const Vector3& cameraPos = CameraComponent3D::ACTIVE->transform->location;

			glUniformMatrix4fv(modelVAO->Viewport(), 1, GL_FALSE, &viewport3D[0][0]);
			glUniform3f(modelVAO->CameraPos(), cameraPos.x, cameraPos.y, cameraPos.z);

			glUniform1i(modelVAO->AmbTex(), 0);
			glUniform1i(modelVAO->DifTex(), 1);
			glUniform1i(modelVAO->SpcTex(), 2);
			glUniform1i(modelVAO->AlpTex(), 3);

			const AmbientLightComponent3D* ambient = AmbientLightComponent3D::ACTIVE;

			if (ambient)
			{
				glUniform3f(modelVAO->AmbLightCol(), ambient->color.r, ambient->color.g, ambient->color.b);
				glUniform1f(modelVAO->AmbLightInt(), ambient->constant);
			}
			else
			{
				glUniform3f(modelVAO->AmbLightCol(), 1.0f, 1.0f, 1.0f);
				glUniform1f(modelVAO->AmbLightInt(), 0.1f);
			}

			const DirectionLightComponent3D* directional = DirectionLightComponent3D::ACTIVE;

			if (directional)
			{
				glUniform3f(modelVAO->DirLightCol(), directional->color.r, directional->color.g, directional->color.b);
				glUniform3f(modelVAO->DirLightDir(), directional->transform->location.x, directional->transform->location.y, directional->transform->location.z);
				glUniform1f(modelVAO->DirLightInt(), directional->constant);
			}
			else
			{
				glUniform3f(modelVAO->DirLightCol(), 1.0f, 1.0f, 1.0f);
				glUniform3f(modelVAO->DirLightDir(), 1.0f, 1.0f, 0.0f);
				glUniform1f(modelVAO->DirLightInt(), 1.0f);
			}
		}

		DrawModelComponent3D::ForEach([&allModels](DrawModelComponent3D* c)
		{
			allModels.push_back(c);
		});

		std::sort(allModels.begin(), allModels.end(), [](const DrawModelComponent3D* l, const DrawModelComponent3D* r)
		{
			if (l->model > r->model) return true;
			if (l->model < r->model) return false;
			
			return false;
		});

		std::map<const Model*, std::vector<const DrawModelComponent3D*>> drawPasses;

		for (auto iModel = allModels.begin(); iModel != allModels.end(); iModel++)
		{
			const DrawModelComponent3D* drawModel = *iModel;
			const Model* model = drawModel->model;

			auto it = drawPasses.find(model);

			if (it == drawPasses.end())
			{
				drawPasses.insert(std::pair<const Model*, std::vector<const DrawModelComponent3D*>>(model, { drawModel }));
			}
			else
			{
				it->second.push_back(drawModel);
			}
		}

		for (auto iPass = drawPasses.begin(); iPass != drawPasses.end(); iPass++)
		{
			const Model* model = iPass->first;
			std::vector<const DrawModelComponent3D*>& instances = iPass->second;

			ByteBuffer instanceBuffer;
			instanceBuffer.Allocate(instances.size() * 128);

			for (auto iInstance = instances.begin(); iInstance != instances.end(); iInstance++)
			{
				const DrawModelComponent3D* instance = *iInstance;

				Matrix4 rotation(CreateRotation(instance->transform->rotation));
				Matrix4 transform = CreateTranslation(instance->transform->location) * /* CreateScale(instance->transform->scale) **/ rotation;

				instanceBuffer.Put(transform);
				instanceBuffer.Put(rotation);
			}

			modelVAO->InstanceBuffer().Bind(GL_ARRAY_BUFFER);
			modelVAO->InstanceBuffer().Fill(instanceBuffer.Data(), instanceBuffer.Size());
			modelVAO->InstanceBuffer().Unbind();

			for (UInt i = 0; i < model->GetMeshes().size(); i++)
			{
				GLBuffer& modelBuffer = modelMap[model][i];
				const Material* material = model->GetMaterials()[i];

				modelVAO->ModelBuffer().Bind(GL_COPY_WRITE_BUFFER);
				modelBuffer.Bind(GL_COPY_READ_BUFFER);

				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, modelBuffer.Size());

				modelVAO->ModelBuffer().Unbind();
				modelBuffer.Unbind();

				glUniform3f(modelVAO->AmbCol(), material->ambient.r, material->ambient.g, material->ambient.b);
				glUniform3f(modelVAO->DifCol(), material->diffuse.r, material->diffuse.g, material->diffuse.b);
				glUniform3f(modelVAO->SpcCol(), material->specular.r, material->specular.g, material->specular.b);
				glUniform1f(modelVAO->SpcExp(), material->exponent);
				glUniform1f(modelVAO->AlpVal(), material->transparency);

				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, textureMap[material->diffuseMap]);

				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, textureMap[material->diffuseMap]);

				glActiveTexture(GL_TEXTURE0 + 2);
				glBindTexture(GL_TEXTURE_2D, textureMap[material->specularMap]);

				glActiveTexture(GL_TEXTURE0 + 3);
				glBindTexture(GL_TEXTURE_2D, textureMap[material->alphaMap]);

				glDrawArraysInstanced(GL_TRIANGLES, 0, model->GetMeshes()[i].size() / 3, instances.size());
			}
		}

		modelVAO->Unbind();
	}
	/*
	void DrawTextures2D()
	{
		std::vector<DrawTextureComponent2D*> textures(0);
		textures.reserve(DrawTextureComponent2D::GetCount());

		if (textures.capacity() == 0) return;

		DrawTextureComponent2D::ForEach([&textures](DrawTextureComponent2D* c)
		{
			textures.push_back(c);
		});

		std::sort(textures.begin(), textures.end(), [](DrawTextureComponent2D* t1, DrawTextureComponent2D* t2)
		{
			//translucency gets priority, followed by depth, followed by texture

			if ((t1->flags & VLK_TRANSLUCENT_BIT) && !(t2->flags & VLK_TRANSLUCENT_BIT)) return false;
			if ((t2->flags & VLK_TRANSLUCENT_BIT) && !(t1->flags & VLK_TRANSLUCENT_BIT)) return true;

			if (t1->depth > t2->depth) return false;
			if (t2->depth > t1->depth) return true;

			return t1->texture > t2->texture;
		});

		ByteBuffer vertices;
		ByteBuffer elements;

		vertices.Allocate(texture2Dvao.vertexBuffer.size);
		elements.Allocate(texture2Dvao.elementBuffer.size);

		for (UInt i = 0; i < textures.size(); i++)
		{
			DrawTextureComponent2D* c = textures[i];

			Vector2 vertPos[4]
			{
				Vector2(c->transform->location.x + c->size.x * (1.0f - c->origin.x),
						c->transform->location.y + c->size.y * (1.0f - c->origin.y)),

				Vector2(c->transform->location.x - c->size.x * c->origin.x,
						c->transform->location.y + c->size.y * (1.0f - c->origin.y)),

				Vector2(c->transform->location.x + c->size.x * (1.0f - c->origin.x),
						c->transform->location.y - c->size.y * c->origin.y),

				Vector2(c->transform->location.x - c->size.x * c->origin.x,
						c->transform->location.y - c->size.y * c->origin.y)
			};

			Vector2 texPos[4]
			{
				Vector2(1, 1),
				Vector2(0, 1),
				Vector2(1, 0),
				Vector2(0, 0)
			};

			if (c->flags & VLK_VERTICAL_FLIP_BIT)
			{
				std::swap(texPos[0], texPos[2]);
				std::swap(texPos[1], texPos[3]);
			}

			if (c->flags & VLK_HORIZONTAL_FLIP_BIT)
			{
				std::swap(texPos[0], texPos[1]);
				std::swap(texPos[2], texPos[3]);
			}

			if (c->transform->rotation != 0.0f)
			{
				for (Byte j = 0; j < 4; j++)
				{
					vertPos[j].RotateAround(c->transform->location, c->transform->rotation);
				}
			}

			if (c->flags & VLK_SCREEN_RELATIVE_BIT)
			{
				for (Byte j = 0; j < 4; j++)
				{
					vertPos[j] += CameraComponent2D::ACTIVE->transform->location;
				}
			}

			for (Byte j = 0; j < 4; j++)
			{
				vertices.Put<Float>(vertPos[j].x);
				vertices.Put<Float>(vertPos[j].y);
				vertices.Put<Float>(texPos[j].x);
				vertices.Put<Float>(texPos[j].y);
				vertices.Put<Float>(c->depth);
				vertices.Put<Float>(c->color.r);
				vertices.Put<Float>(c->color.g);
				vertices.Put<Float>(c->color.b);
				vertices.Put<Float>(c->color.a);
			}


			elements.Put((i * 4) + 0);
			elements.Put((i * 4) + 1);
			elements.Put((i * 4) + 2);
			elements.Put((i * 4) + 1);
			elements.Put((i * 4) + 2);
			elements.Put((i * 4) + 3);
		}

		texture2Dvao.Bind();
		texture2Dvao.elementBuffer.Fill(elements);
		texture2Dvao.vertexBuffer.Fill(vertices);

		glUniformMatrix4fv(texture2Dvao.viewportBinding, 1, GL_FALSE, viewport2D.Data());

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_CULL_FACE);

		UInt lastDraw = 0;
		UInt thisTex = 0;

		for (auto it = textures.begin(); it != textures.end(); it++)
		{
			thisTex++;

			auto nextIt = it + 1;

			if (nextIt != textures.end())
			{
				if ((*nextIt)->texture == (*it)->texture)
				{
					continue;
				}
			}

			glBindTexture(GL_TEXTURE_2D, textureMap[(*it)->texture]);
			glDrawElements(GL_TRIANGLES, 6 * (thisTex - lastDraw), GL_UNSIGNED_INT, reinterpret_cast<void*>(static_cast<ULong>(lastDraw) * 6 * 4));
			//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(static_cast<ULong>(lastDraw) * 6 * 4), thisTex - lastDraw);
			lastDraw = thisTex;
		}

		texture2Dvao.Unbind();
	}*/
}

void GLRenderer::Init()
{
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	modelVAO = new ModelVAO();
	
	EventBus<vlk::WindowFramebufferEvent>::Get().AddEventListener(OnFramebufferResize);

	EventBus<vlk::ContentLoadedEvent<Texture2D>>::Get().AddEventListener(OnTextureLoad);
	EventBus<vlk::ContentUnloadedEvent<Texture2D>>::Get().AddEventListener(OnTextureUnload);

	EventBus<vlk::ContentLoadedEvent<Model>>::Get().AddEventListener(OnModelLoad);
	EventBus<vlk::ContentUnloadedEvent<Model>>::Get().AddEventListener(OnModelUnload);
}

void GLRenderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	viewport2D = CameraComponent2D::ACTIVE->GetView() * CameraComponent2D::ACTIVE->GetProjection();

	viewport3D =
		CameraComponent3D::ACTIVE->GetProjection() *
		CameraComponent3D::ACTIVE->GetView();// *
		//CreateTranslation(CameraComponent3D::ACTIVE->transform->location);

	DrawModels3D();
	//DrawTextures2D();
	/*
	DrawStrings();

	DrawPrimitives2D();*/
}

void GLRenderer::Destroy()
{
	//texture2Dvao.Delete();
	delete modelVAO;

	/*
	DestroyPrimitive2DVertexArray();
	DestroyTexture2DVertexArray();

	EventBus<vlk::WindowFramebufferEvent>::Get().RemoveEventListener(OnResize);*/
}