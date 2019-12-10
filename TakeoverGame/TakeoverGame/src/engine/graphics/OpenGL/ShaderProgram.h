#pragma once

#include "../../core/ValkyrieEngine.h"

#include "Shader.h"

namespace vlk
{
	struct ShaderProgram
	{
		ShaderProgram();
		~ShaderProgram();

		void AttachShader(Shader& shader);
		void Link();

		UInt GetAttribLocation(const std::string& name) const;
		UInt GetUniformLocation(const std::string& name) const;

		void MakeUsed();
		void MakeUnused();

		inline UInt Handle() const { return handle; }

		private:
		UInt handle;
	};
}