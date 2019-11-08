#pragma once

#include "../../core/ValkyrieEngine.h"
#include "../../utils/ByteBuffer.hpp"

namespace vlk
{
	struct VertexBuffer
	{
		VertexBuffer(UInt _type);

		inline operator const UInt() const { return handle; }

		void Fill(const ByteBuffer& b);

		UInt handle;
		UInt size;
		const UInt type;
	};

	struct VertexArray
	{
		virtual void Create() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Delete() = 0;
		virtual UInt GetViewportBinding() const = 0;
	};

	struct Texture2DVAO : VertexArray
	{
		Texture2DVAO();

		Texture2DVAO(const Texture2DVAO& other) = delete;
		Texture2DVAO(Texture2DVAO&& other) = delete;
		Texture2DVAO& operator=(const Texture2DVAO& other) = delete;

		void Create() override;
		void Bind() override;
		void Unbind() override;
		void Delete() override;
		UInt GetViewportBinding() const override;

		UInt shaderProgram;
		UInt vao;
		UInt viewportBinding;

		//Element
		VertexBuffer elementBuffer;

		//X Y position
		//depth
		//RGBA
		VertexBuffer vertexBuffer;

	};
}