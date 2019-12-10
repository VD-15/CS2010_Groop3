#pragma once

#include "../../core/ValkyrieEngine.h"
#include "../../utils/ByteBuffer.hpp"

#include "Buffer.h"
#include "ShaderProgram.h"

namespace vlk
{
	struct ModelVAO
	{
		ModelVAO();
		~ModelVAO();

		ModelVAO(const ModelVAO& other) = delete;
		ModelVAO(ModelVAO&& other) = delete;
		ModelVAO& operator=(const ModelVAO& other) = delete;

		void Bind();
		void Unbind();

		inline GLBuffer& ModelBuffer() { return modelBuffer; }
		inline GLBuffer& InstanceBuffer() { return instanceBuffer; }

		inline UInt Viewport() const { return this->viewportBinding; }
		inline UInt CameraPos() const { return this->cameraPosBinding; }

		inline UInt AmbTex() const { return this->ambTexBinding; }
		inline UInt DifTex() const { return this->difTexBinding; }
		inline UInt SpcTex() const { return this->spcTexBinding; }
		inline UInt AlpTex() const { return this->alpTexBinding; }

		inline UInt AmbCol() const { return this->ambBinding; }
		inline UInt DifCol() const { return this->difBinding; }
		inline UInt SpcCol() const { return this->spcBinding; }
		inline UInt SpcExp() const { return this->expBinding; }
		inline UInt AlpVal() const { return this->alpBinding; }

		inline UInt AmbLightCol() const { return this->ambLightColBinding; }
		inline UInt AmbLightInt() const { return this->ambLightIntBinding; }

		inline UInt DirLightCol() const { return this->dirLightColBinding; }
		inline UInt DirLightInt() const { return this->dirLightIntBinding; }
		inline UInt DirLightDir() const { return this->dirLightDirBinding; }

		private:
		ShaderProgram shaderProgram;
		UInt handle;

		GLBuffer modelBuffer;
		GLBuffer instanceBuffer;

		UInt viewportBinding;
		UInt cameraPosBinding;

		UInt ambTexBinding;
		UInt difTexBinding;
		UInt spcTexBinding;
		UInt alpTexBinding;

		UInt ambBinding;
		UInt difBinding;
		UInt spcBinding;
		UInt expBinding;
		UInt alpBinding;

		UInt ambLightColBinding;
		UInt ambLightIntBinding;

		UInt dirLightColBinding;
		UInt dirLightIntBinding;
		UInt dirLightDirBinding;
	};
}