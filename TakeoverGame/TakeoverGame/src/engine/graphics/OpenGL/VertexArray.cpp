#include "VertexArray.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

#include "../../utils/VLKIO.h"

using namespace vlk;

namespace
{
	//Maximum number of texture draws at once
	constexpr UInt MAX_TEXTURES = 1024;
	constexpr UInt MAX_MESH_SIZE = 16777216; 
	constexpr UInt MAX_MODEL_INSTANCES = 32768;

	UInt CreateShader(UInt type, const std::string& path)
	{
		UInt shader = glCreateShader(type);
		StringCollection src = IO::ReadFile(path);
		glShaderSource(shader, src.Count(), src.Data(), src.Lengths());
		glCompileShader(shader);
		return shader;
	}

	void CheckShaderCompile(UInt shader)
	{
		Int success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			Int logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			char* infoLog = new char[logLength];
			glGetShaderInfoLog(shader, logLength, 0, infoLog);
			LogError("GLRenderer", "Shader failed to compile:\n" + std::string(infoLog));
			delete[] infoLog;
		}
		else
		{
			LogVerbose("GLRenderer", "Shader compiled successfully");
		}
	}

	void CheckProgramLink(UInt program)
	{
		Int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success)
		{
			Int logLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

			char* infoLog = new char[logLength];
			glGetProgramInfoLog(program, logLength, 0, infoLog);
			LogError("GLRenderer", "Program failed to link:\n" + std::string(infoLog));
			delete[] infoLog;
		}
		else
		{
			LogVerbose("GLRenderer", "Program compiled successfully");
		}
	}
}

GLBuffer::GLBuffer(UInt _type) :
	type(_type)
{
	this->handle = 0;
	this->size = 0;
}

void GLBuffer::Fill(const ByteBuffer& b)
{
	glBindBuffer(this->type, this->handle);
	
	#ifdef _DEBUG
	if (b.Size() > this->size)
	{
		LogError("VertexArray", "Buffer size larger than allowed.");
		throw std::exception("Buffer size larger than allowed.");
	}
	#endif

	glBufferSubData(this->type, 0, b.Size(), b.Data());
}

Texture2DVAO::Texture2DVAO() :
	elementBuffer(GL_ELEMENT_ARRAY_BUFFER),
	vertexBuffer(GL_ARRAY_BUFFER)
{
	this->shaderProgram = 0;
	this->viewportBinding = 0;
	this->vao = 0;
}

void Texture2DVAO::Create()
{
	UInt buffers[2];
	glGenBuffers(2, &buffers[0]);

	this->elementBuffer.handle = buffers[0];
	this->vertexBuffer.handle = buffers[1];

	this->vertexBuffer.size = MAX_TEXTURES * ((2 + 2 + 1 + 4) * 4) * 4;
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer.handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertexBuffer.size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->elementBuffer.size = MAX_TEXTURES * (6 * 4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer.handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer.size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	UInt vShader = CreateShader(GL_VERTEX_SHADER, "shaders/texture2D.vert");
	UInt fShader = CreateShader(GL_FRAGMENT_SHADER, "shaders/texture2D.frag");

	CheckShaderCompile(vShader);
	CheckShaderCompile(fShader);

	this->shaderProgram = glCreateProgram();

	glAttachShader(this->shaderProgram, vShader);
	glAttachShader(this->shaderProgram, fShader);
	glLinkProgram(this->shaderProgram);

	CheckProgramLink(this->shaderProgram);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glUseProgram(this->shaderProgram);

	UInt locAttrib = glGetAttribLocation(this->shaderProgram, "inPosition");
	UInt texAttrib = glGetAttribLocation(this->shaderProgram, "inUV");
	UInt depAttrib = glGetAttribLocation(this->shaderProgram, "inDepth");
	UInt colAttrib = glGetAttribLocation(this->shaderProgram, "inColor");

	this->viewportBinding = glGetUniformLocation(this->shaderProgram, "uViewport");

	glEnableVertexAttribArray(locAttrib);
	glEnableVertexAttribArray(depAttrib);
	glEnableVertexAttribArray(colAttrib);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer.handle);
	glVertexAttribPointer(locAttrib, 2, GL_FLOAT, GL_FALSE, 9 * 4, reinterpret_cast<void*>(0ULL * 4ULL));
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * 4, reinterpret_cast<void*>(2ULL * 4ULL));
	glVertexAttribPointer(depAttrib, 1, GL_FLOAT, GL_FALSE, 9 * 4, reinterpret_cast<void*>(4ULL * 4ULL));
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * 4, reinterpret_cast<void*>(5ULL * 4ULL));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Texture2DVAO::Bind()
{
	glBindVertexArray(this->vao);
	glUseProgram(this->shaderProgram);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
}

void Texture2DVAO::Unbind()
{
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Texture2DVAO::Delete()
{
	UInt buffers[2] { this->elementBuffer, this->vertexBuffer };
	glDeleteBuffers(2, &buffers[0]);

	glDeleteProgram(this->shaderProgram);
	glDeleteVertexArrays(1, &this->vao);
}

ModelVAO::ModelVAO() :
	modelBuffer(GL_ARRAY_BUFFER),
	instanceBuffer(GL_ARRAY_BUFFER)
{
	this->shaderProgram = 0;
	this->vao = 0;
	this->viewportBinding = 0;
	this->transformBinding = 0;

	this->ambBinding = 0;
	this->difBinding = 0;
	this->spcBinding = 0;
	this->expBinding = 0;
	this->alpBinding = 0;

	this->ambLightColBinding = 0;
	this->ambLightIntBinding = 0;

	this->pntLightColBinding = 0;
	this->pntLightIntBinding = 0;
	this->pntLightPosBinding = 0;
	this->pntLightNumBinding = 0;

	this->dirLightColBinding = 0;
	this->dirLightIntBinding = 0;
	this->dirLightDirBinding = 0;
	this->dirLightNumBinding = 0;
}

void ModelVAO::Create()
{
	UInt buffers[2];
	glGenBuffers(2, &buffers[0]);

	this->modelBuffer.handle = buffers[0];
	this->instanceBuffer.handle = buffers[1];

	this->modelBuffer.size = MAX_MESH_SIZE * (2 + 3 + 3) * 4; //UV + pos + norm = 8
	glBindBuffer(GL_ARRAY_BUFFER, this->modelBuffer.handle);
	glBufferData(GL_ARRAY_BUFFER, this->modelBuffer.size, nullptr, GL_DYNAMIC_DRAW);

	this->instanceBuffer.size = MAX_MODEL_INSTANCES * (16) * 4; //transform matrix
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceBuffer.handle);
	glBufferData(GL_ARRAY_BUFFER, this->instanceBuffer.size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	UInt vShader = CreateShader(GL_VERTEX_SHADER, "shaders/model3D.vert");
	UInt fShader = CreateShader(GL_FRAGMENT_SHADER, "shaders/model3D.frag");

	CheckShaderCompile(vShader);
	CheckShaderCompile(fShader);

	this->shaderProgram = glCreateProgram();

	glAttachShader(this->shaderProgram, vShader);
	glAttachShader(this->shaderProgram, fShader);
	glLinkProgram(this->shaderProgram);

	CheckProgramLink(this->shaderProgram);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glUseProgram(this->shaderProgram);

	//per vertex attribs
	UInt posAttrib = glGetAttribLocation(this->shaderProgram, "inPos");
	UInt texAttrib = glGetAttribLocation(this->shaderProgram, "inUV");
	UInt nmlAttrib = glGetAttribLocation(this->shaderProgram, "inNormal");

	//per instance attribs
	UInt trfAttrib = glGetAttribLocation(this->shaderProgram, "inTransform");

	//material uniforms
	this->ambBinding = glGetUniformLocation(this->shaderProgram, "uAmbient");
	this->difBinding = glGetUniformLocation(this->shaderProgram, "uDiffuse");
	this->spcBinding = glGetUniformLocation(this->shaderProgram, "uSpecular");
	this->alpBinding = glGetUniformLocation(this->shaderProgram, "uAlpha");
	this->expBinding = glGetUniformLocation(this->shaderProgram, "uExponent");

	this->ambLightColBinding = glGetUniformLocation(this->shaderProgram, "uAmbientColor");
	this->ambLightIntBinding = glGetUniformLocation(this->shaderProgram, "uAmbientIntensity");

	this->cameraPosBinding = glGetUniformLocation(this->shaderProgram, "uViewPos");

	this->pntLightColBinding = glGetUniformLocation(this->shaderProgram, "uPointColor");
	this->pntLightIntBinding = glGetUniformLocation(this->shaderProgram, "uPointIntensity");
	this->pntLightPosBinding = glGetUniformLocation(this->shaderProgram, "uPointLocation");
	this->pntLightNumBinding = glGetUniformLocation(this->shaderProgram, "uPointCount");

	this->dirLightColBinding = glGetUniformLocation(this->shaderProgram, "uDirectionColor");
	this->dirLightIntBinding = glGetUniformLocation(this->shaderProgram, "uDirectionIntensity");
	this->dirLightDirBinding = glGetUniformLocation(this->shaderProgram, "uDirectionVector");
	this->dirLightNumBinding = glGetUniformLocation(this->shaderProgram, "uDirectionCount");

	//viewport uniform
	this->viewportBinding = glGetUniformLocation(this->shaderProgram, "uViewport");

	//enable per-vertex attributes
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(texAttrib);
	glEnableVertexAttribArray(nmlAttrib);

	//enable per-element attributes
	glEnableVertexAttribArray(trfAttrib + 0);
	glEnableVertexAttribArray(trfAttrib + 1);
	glEnableVertexAttribArray(trfAttrib + 2);
	glEnableVertexAttribArray(trfAttrib + 3);

	glBindBuffer(GL_ARRAY_BUFFER, this->modelBuffer.handle);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(0ULL * 4ULL));
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(3ULL * 4ULL));
	glVertexAttribPointer(nmlAttrib, 3, GL_FLOAT, GL_FALSE, 8 * 4, reinterpret_cast<void*>(5ULL * 4ULL));

	glVertexAttribDivisor(posAttrib, 0);
	glVertexAttribDivisor(texAttrib, 0);
	glVertexAttribDivisor(nmlAttrib, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->instanceBuffer.handle);
	glVertexAttribPointer(trfAttrib + 0, 4, GL_FLOAT, GL_FALSE, 16 * 4, reinterpret_cast<void*>( 0ULL * 4ULL));
	glVertexAttribPointer(trfAttrib + 1, 4, GL_FLOAT, GL_FALSE, 16 * 4, reinterpret_cast<void*>( 4ULL * 4ULL));
	glVertexAttribPointer(trfAttrib + 2, 4, GL_FLOAT, GL_FALSE, 16 * 4, reinterpret_cast<void*>( 8ULL * 4ULL));
	glVertexAttribPointer(trfAttrib + 3, 4, GL_FLOAT, GL_FALSE, 16 * 4, reinterpret_cast<void*>(12ULL * 4ULL));

	glVertexAttribDivisor(trfAttrib + 0, 1);
	glVertexAttribDivisor(trfAttrib + 1, 1);
	glVertexAttribDivisor(trfAttrib + 2, 1);
	glVertexAttribDivisor(trfAttrib + 3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void ModelVAO::Bind()
{
	glBindVertexArray(this->vao);
	glUseProgram(this->shaderProgram);
}

void ModelVAO::Unbind()
{
	glBindVertexArray(0);
	glUseProgram(0);
}

void ModelVAO::Delete()
{
	UInt buffers[2] { this->modelBuffer, this->instanceBuffer };
	glDeleteBuffers(2, &buffers[0]);

	glDeleteProgram(this->shaderProgram);
	glDeleteVertexArrays(1, &this->vao);
}