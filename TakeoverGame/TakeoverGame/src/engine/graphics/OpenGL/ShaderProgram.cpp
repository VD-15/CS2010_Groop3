#include "ShaderProgram.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

using namespace vlk;

ShaderProgram::ShaderProgram()
{
	this->handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->handle);
}

void ShaderProgram::AttachShader(Shader& shader)
{
	glAttachShader(this->handle, shader.Handle());
}

void ShaderProgram::Link()
{
	glLinkProgram(this->handle);

	#ifdef _DEBUG
	Int success = 0;
	glGetProgramiv(this->handle, GL_LINK_STATUS, &success);

	if (!success)
	{
		Int logLength = 0;
		glGetProgramiv(this->handle, GL_INFO_LOG_LENGTH, &logLength);

		char* infoLog = new char[logLength];
		glGetProgramInfoLog(this->handle, logLength, 0, infoLog);
		LogError("GLRenderer", "Program failed to link:\n" + std::string(infoLog));
		delete[] infoLog;
	}
	else
	{
		LogVerbose("GLRenderer", "Program compiled successfully");
	}
	#endif
}

UInt ShaderProgram::GetAttribLocation(const std::string& name) const
{
	UInt location = glGetAttribLocation(this->handle, name.c_str());

	#ifdef _DEBUG
	if (location == std::numeric_limits<UInt>::max())
	{
		LogError("ShaderProgram", "Failed to find attribute location: " + name);
	}
	#endif

	return location;
}

UInt ShaderProgram::GetUniformLocation(const std::string& name) const
{
	UInt location = glGetUniformLocation(this->handle, name.c_str());

	#ifdef _DEBUG
	if (location == std::numeric_limits<UInt>::max())
	{
		LogError("ShaderProgram", "Failed to find uniform location: " + name);
	}
	#endif

	return location;
}

void ShaderProgram::MakeUsed()
{
	glUseProgram(this->handle);
}

void ShaderProgram::MakeUnused()
{
	glUseProgram(0);
}