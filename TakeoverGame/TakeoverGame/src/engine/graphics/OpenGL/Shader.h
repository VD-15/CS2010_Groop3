#pragma once

#include "../../core/ValkyrieEngine.h"

namespace vlk
{
	struct Shader
	{
		Shader(UInt shaderType);
		~Shader();

		void LoadSource(const std::string& path);
		void Compile();

		inline UInt Handle() const { return handle; }

		private:
		const UInt handle;
	};

}