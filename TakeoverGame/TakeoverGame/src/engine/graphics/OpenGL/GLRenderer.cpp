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

	std::map<const Texture2D*, UInt> textureMap;
	Texture2DVAO texture2Dvao;
	ModelVAO modelVAO;

	constexpr UInt MAX_POINT_LIGHTS = 64;
	constexpr UInt MAX_DRCTN_LIGHTS = 4;

	void OnFramebufferResize(vlk::WindowFramebufferEvent& ev)
	{
		glViewport(0, 0, ev.width, ev.height);
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
			genMipMapEnum = 1;

			if (filterMode == "nearest") 
			{ 
				filterEnumMin = GL_NEAREST; 
				filterEnumMax = GL_NEAREST; 
			}
		}
		else
		{
			filterEnumMax = GL_LINEAR_MIPMAP_LINEAR;
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
		std::vector<DrawModelComponent3D*> models(0);
		models.reserve(DrawModelComponent3D::GetCount());

		if (models.capacity() == 0) return;

		std::set<const Model*> uniqueModels;

		DrawModelComponent3D::ForEach([&models, &uniqueModels](DrawModelComponent3D* c)
		{
			models.push_back(c);
			uniqueModels.emplace(c->model);
		});

		//sort by models
		std::sort(models.begin(), models.end(), [](DrawModelComponent3D* m1, DrawModelComponent3D* m2)
		{
			return m1->model > m2->model;
		});

		std::vector<DrawModelComponent3D*> thisDraw;
		thisDraw.reserve(models.size());

		modelVAO.Bind();
		glUniformMatrix4fv(modelVAO.viewportBinding, 1, GL_FALSE, viewport3D.Data());

		//bind light uniforms
		{
			std::vector<float> pointCol;
			std::vector<float> pointInt;
			std::vector<float> pointLoc;

			std::vector<Float> dirCol;
			std::vector<Float> dirInt;
			std::vector<Float> dirDir;

			pointCol.reserve(MAX_POINT_LIGHTS * 3);
			pointInt.reserve(MAX_POINT_LIGHTS * 3);
			pointLoc.reserve(MAX_POINT_LIGHTS * 3);
			UInt pointCount = 0;

			dirCol.reserve(MAX_DRCTN_LIGHTS * 3);
			dirInt.reserve(MAX_DRCTN_LIGHTS * 3);
			dirDir.reserve(MAX_DRCTN_LIGHTS * 3);
			UInt dirCount = 0;

			PointLightComponent3D::ForEach([&pointCol, &pointInt, &pointLoc, &pointCount](PointLightComponent3D* c)
			{
				if (pointCount < MAX_POINT_LIGHTS)
				{
					//push light color
					pointCol.push_back(c->color.r);
					pointCol.push_back(c->color.g);
					pointCol.push_back(c->color.b);

					//push light intensity falloff
					pointInt.push_back(c->quadratic);
					pointInt.push_back(c->linear);
					pointInt.push_back(c->constant);

					//Push point light location
					pointLoc.push_back(c->transform->location.x);
					pointLoc.push_back(c->transform->location.y);
					pointLoc.push_back(c->transform->location.z);

					pointCount++;
				}
			});

			DirectionLightComponent3D::ForEach([&dirCol, &dirInt, &dirDir, &dirCount](DirectionLightComponent3D* c)
			{
				if (dirCount < MAX_DRCTN_LIGHTS)
				{
					//push light color
					dirCol.push_back(c->color.r);
					dirCol.push_back(c->color.g);
					dirCol.push_back(c->color.b);

					//push light intensity falloff
					dirInt.push_back(c->quadratic);
					dirInt.push_back(c->linear);
					dirInt.push_back(c->constant);

					//puish direction of light
					Vector3 v(Quaternion::Rotate(Vector3::DOWN, c->transform->rotation));

					dirDir.push_back(v.x);
					dirDir.push_back(v.y);
					dirDir.push_back(v.z);

					dirCount++;
				}
			});

			//Camera position
			glUniform3f(modelVAO.cameraPosBinding, CameraComponent3D::ACTIVE->transform->location.x, CameraComponent3D::ACTIVE->transform->location.y, CameraComponent3D::ACTIVE->transform->location.z);

			//Point lights
			glUniform1i(modelVAO.pntLightNumBinding, pointCount);
			glUniform3fv(modelVAO.pntLightColBinding, pointCount, pointCol.data());
			glUniform3fv(modelVAO.pntLightIntBinding, pointCount, pointInt.data());
			glUniform3fv(modelVAO.pntLightPosBinding, pointCount, pointLoc.data());

			//Directional lights
			glUniform1i(modelVAO.dirLightNumBinding, dirCount);
			glUniform3fv(modelVAO.dirLightColBinding, dirCount, dirCol.data());
			glUniform3fv(modelVAO.dirLightIntBinding, dirCount, dirInt.data());
			glUniform3fv(modelVAO.dirLightDirBinding, dirCount, dirDir.data());

			//Ambient Light
			if (AmbientLightComponent3D::ACTIVE)
			{
				glUniform3f(modelVAO.ambLightColBinding, AmbientLightComponent3D::ACTIVE->color.r, AmbientLightComponent3D::ACTIVE->color.g, AmbientLightComponent3D::ACTIVE->color.b);
				glUniform1f(modelVAO.ambLightIntBinding, AmbientLightComponent3D::ACTIVE->constant);
			}
			else
			{
				glUniform3f(modelVAO.ambLightColBinding, 1.0f, 1.0f, 1.0f);
				glUniform1f(modelVAO.ambLightIntBinding, 0.0f);
			}
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		//glCullFace(GL_FRONT);

		//Draw unique models in groups
		for (auto iModel = uniqueModels.begin(); iModel != uniqueModels.end(); iModel++)
		{
			const Model* model = *iModel;

			//Get all DrawModelComponents that use this model and move them into thisDraw
			{
				thisDraw.clear();
				auto it = std::remove_if(models.begin(), models.end(), [model](const DrawModelComponent3D* d)
				{
					return d->model == model;
				});

				//Move components from models to thisDraw
				std::move(it, models.end(), std::back_inserter(thisDraw));
				models.erase(it, models.end());
			}

			//Contains all the transforms for the model renders
			ByteBuffer perInstanceBuffer;
			perInstanceBuffer.Allocate(thisDraw.size() * 16 * 4);

			//Load transform matrices into perInstanceBuffer
			for (auto it = thisDraw.begin(); it != thisDraw.end(); it++)
			{
				DrawModelComponent3D* c = *it;

				Matrix4 transform =		
										Matrix4::CreateScale(c->transform->scale) *
										Matrix4::CreateTranslation(c->transform->location) *
										Matrix4::CreateRotation(c->transform->rotation);

				for (UInt i = 0; i < 16; i++)
				{
					perInstanceBuffer.Put<Float>(transform.Data()[i]);
				}
			}

			//draw meshes
			for (ULong i = 0; i < model->GetMeshes().size(); i++)
			{
				const std::vector<UInt>& mesh = model->GetMeshes()[i];
				const Material* material = model->GetMaterials()[i];

				//Contains the mesh data
				ByteBuffer perVertexBuffer;
				perVertexBuffer.Allocate(mesh.size() * (3 * 2 * 3) * 4);

				//Assemble mesh data into perVertexBuffer
				for (ULong j = 0; j < mesh.size(); j += 3)
				{
					ULong vertex = static_cast<ULong>(mesh[j + 0] - 1) * 3;
					ULong coord = static_cast<ULong>(mesh[j + 1] - 1) * 2;
					ULong norm = static_cast<ULong>(mesh[j + 2] - 1) * 3;

					perVertexBuffer.Put<Float>(model->GetVertices()[vertex + 0]);
					perVertexBuffer.Put<Float>(model->GetVertices()[vertex + 1]);
					perVertexBuffer.Put<Float>(model->GetVertices()[vertex + 2]);

					perVertexBuffer.Put<Float>(model->GetCoords()[coord + 0]);
					perVertexBuffer.Put<Float>(model->GetCoords()[coord + 1]);

					perVertexBuffer.Put<Float>(model->GetNormals()[norm + 0]);
					perVertexBuffer.Put<Float>(model->GetNormals()[norm + 1]);
					perVertexBuffer.Put<Float>(model->GetNormals()[norm + 2]);
				}

				modelVAO.modelBuffer.Fill(perVertexBuffer);
				modelVAO.instanceBuffer.Fill(perInstanceBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				//Send material to uniforms
				{
					glActiveTexture(GL_TEXTURE0 + 0);
					glBindTexture(GL_TEXTURE_2D, textureMap[material->diffuseMap]);

					glActiveTexture(GL_TEXTURE0 + 1);
					glBindTexture(GL_TEXTURE_2D, textureMap[material->specularMap]);

					glActiveTexture(GL_TEXTURE0 + 2);
					glBindTexture(GL_TEXTURE_2D, textureMap[material->exponentMap]);

					glActiveTexture(GL_TEXTURE0 + 3);
					glBindTexture(GL_TEXTURE_2D, textureMap[material->alphaMap]);

					glUniform3f(modelVAO.ambBinding, material->ambient.r, material->ambient.g, material->ambient.b);
					glUniform3f(modelVAO.difBinding, material->diffuse.r, material->diffuse.g, material->diffuse.b);
					glUniform3f(modelVAO.spcBinding, material->specular.r, material->specular.g, material->specular.b);
					glUniform1f(modelVAO.expBinding, material->exponent);
					glUniform1f(modelVAO.alpBinding, material->transparency);
				}

				glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<UInt>(mesh.size() / 3), static_cast<UInt>(thisDraw.size()));
			}
		}

		modelVAO.Unbind();
	}

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
	}
}

void GLRenderer::Init()
{
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	texture2Dvao.Create();
	modelVAO.Create();
	
	EventBus<vlk::WindowFramebufferEvent>::Get().AddEventListener(OnFramebufferResize);
	EventBus<vlk::ContentLoadedEvent<Texture2D>>::Get().AddEventListener(OnTextureLoad);
	EventBus<vlk::ContentUnloadedEvent<Texture2D>>::Get().AddEventListener(OnTextureUnload);
}

void GLRenderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	viewport2D = CameraComponent2D::ACTIVE->GetView() * CameraComponent2D::ACTIVE->GetProjection();
	viewport3D = CameraComponent3D::ACTIVE->GetView() * CameraComponent3D::ACTIVE->GetProjection();

	DrawModels3D();
	DrawTextures2D();
	/*
	DrawStrings();

	DrawPrimitives2D();*/
}

void GLRenderer::Destroy()
{
	texture2Dvao.Delete();
	modelVAO.Delete();

	/*
	DestroyPrimitive2DVertexArray();
	DestroyTexture2DVertexArray();

	EventBus<vlk::WindowFramebufferEvent>::Get().RemoveEventListener(OnResize);*/
}