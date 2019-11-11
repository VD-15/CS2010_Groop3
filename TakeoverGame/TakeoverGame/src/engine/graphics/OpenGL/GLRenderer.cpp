#include "GLRenderer.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

#include "utf8/utf8.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "../../components/DrawPrimitiveComponent.h"
#include "../../components/DrawTextureComponent.h"
#include "../../components/DrawStringComponent.h"
#include "../../components/CameraComponent.h"
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
	
	EventBus<vlk::WindowFramebufferEvent>::Get().AddEventListener(OnFramebufferResize);
	EventBus<vlk::ContentLoadedEvent<Texture2D>>::Get().AddEventListener(OnTextureLoad);
	EventBus<vlk::ContentUnloadedEvent<Texture2D>>::Get().AddEventListener(OnTextureUnload);
}

void GLRenderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	viewport2D = CameraComponent2D::ACTIVE->GetView() * CameraComponent2D::ACTIVE->GetProjection();

	DrawTextures2D();
	/*
	DrawStrings();

	DrawPrimitives2D();*/
}

void GLRenderer::Destroy()
{/*
	DestroyPrimitive2DVertexArray();
	DestroyTexture2DVertexArray();

	EventBus<vlk::WindowFramebufferEvent>::Get().RemoveEventListener(OnResize);*/
}