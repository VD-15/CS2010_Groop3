#pragma once

#include "../../core/ValkyrieEngine.h"
#include "../../utils/ByteBuffer.hpp"

namespace vlk
{
	struct GLBuffer
	{
		GLBuffer(UInt _type);

		GLBuffer(const GLBuffer& other);
		GLBuffer(GLBuffer&& other) noexcept;

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

		UInt shaderProgram;
		UInt vao;
		UInt viewportBinding;

		//Element
		GLBuffer elementBuffer;

		//X Y position
		//depth
		//RGBA
		GLBuffer vertexBuffer;
	};

	struct ModelVAO : VertexArray
	{
		ModelVAO();

		ModelVAO(const ModelVAO& other) = delete;
		ModelVAO(ModelVAO&& other) = delete;
		ModelVAO& operator=(const ModelVAO& other) = delete;

		void Create() override;
		void Bind() override;
		void Unbind() override;
		void Delete() override;

		UInt shaderProgram;
		UInt vao;

		//per draw viewport matrix binding
		UInt viewportBinding;

		//per instance transform matrix binding
		UInt transformBinding;

		//Per material diffuse color bindings:

		//Ambient color binding
		UInt ambBinding;

		//diffuse color binding
		UInt difBinding;

		//specular color binding
		UInt spcBinding;

		//specular exponent binding
		UInt expBinding;

		//transparency binding
		UInt alpBinding;

		//Ambient light color binding
		UInt ambLightColBinding;

		//Ambient light intensity binding
		UInt ambLightIntBinding;

		//Camera position
		UInt cameraPosBinding;

		UInt pntLightColBinding;
		UInt pntLightIntBinding;
		UInt pntLightPosBinding;
		UInt pntLightNumBinding;

		UInt dirLightColBinding;
		UInt dirLightIntBinding;
		UInt dirLightDirBinding;
		UInt dirLightNumBinding;

		GLBuffer modelBuffer;

		GLBuffer instanceBuffer;
	};
}