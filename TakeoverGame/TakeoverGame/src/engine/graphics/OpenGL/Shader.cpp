#include "Shader.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

#include "../../utils/StringCollection.h"
#include "../../utils/VLKIO.h"

using namespace vlk;

Shader::Shader(UInt shaderType) :
	handle(glCreateShader(shaderType))
{

}

Shader::~Shader()
{
	glDeleteShader(this->handle);
}

void Shader::LoadSource(const std::string& path)
{
	StringCollection src = IO::ReadFile(path);
	glShaderSource(this->handle, src.Count(), src.Data(), src.Lengths());
}

void Shader::Compile()
{
	glCompileShader(this->handle);

	#ifdef _DEBUG
	Int success = 0;
	glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		Int logLength = 0;
		glGetShaderiv(this->handle, GL_INFO_LOG_LENGTH, &logLength);

		char* infoLog = new char[logLength];
		glGetShaderInfoLog(this->handle, logLength, 0, infoLog);
		LogError("GLRenderer", "Shader failed to compile:\n" + std::string(infoLog));
		delete[] infoLog;
	}
	else
	{
		LogVerbose("GLRenderer", "Shader compiled successfully");
	}
	#endif
}