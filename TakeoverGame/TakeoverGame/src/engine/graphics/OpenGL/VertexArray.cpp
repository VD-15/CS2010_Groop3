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
}

ModelVAO::ModelVAO()
{
	this->handle = 0;
	glGenVertexArrays(1, &this->handle);
	glBindVertexArray(this->handle);

	Shader vertShader(GL_VERTEX_SHADER);
	Shader fragShader(GL_FRAGMENT_SHADER);

	vertShader.LoadSource("shaders/model3D.vert");
	fragShader.LoadSource("shaders/model3D.frag");

	vertShader.Compile();
	fragShader.Compile();

	this->shaderProgram.AttachShader(vertShader);
	this->shaderProgram.AttachShader(fragShader);

	this->shaderProgram.Link();

	{
		this->instanceBuffer.Bind(GL_ARRAY_BUFFER);
		this->instanceBuffer.Resize(MAX_MODEL_INSTANCES * 128, GL_DYNAMIC_DRAW);

		UInt transformBinding = this->shaderProgram.GetAttribLocation("inTransform");
		UInt rotationBinding = this->shaderProgram.GetAttribLocation("inRotation");

		for (Byte i = 0; i < 4; i++)
		{
			glEnableVertexAttribArray(transformBinding + i);
			glEnableVertexAttribArray(rotationBinding + i);

			glVertexAttribPointer(transformBinding + i, 4, GL_FLOAT, GL_FALSE, 128, reinterpret_cast<void*>(16ULL * i));
			glVertexAttribPointer(rotationBinding + i , 4, GL_FLOAT, GL_FALSE, 128, reinterpret_cast<void*>(64ULL + 16ULL * i));

			glVertexBindingDivisor(transformBinding + i, 1);
			glVertexBindingDivisor(rotationBinding + i, 1);
		}

		this->instanceBuffer.Unbind();
	}

	{
		this->modelBuffer.Bind(GL_ARRAY_BUFFER);
		this->modelBuffer.Resize(MAX_MESH_SIZE, GL_DYNAMIC_COPY);

		UInt posBinding = this->shaderProgram.GetAttribLocation("inPos");
		UInt texBinding = this->shaderProgram.GetAttribLocation("inTex");
		UInt nmlBinding = this->shaderProgram.GetAttribLocation("inNml");

		glEnableVertexAttribArray(posBinding);
		glEnableVertexAttribArray(texBinding);
		glEnableVertexAttribArray(nmlBinding);

		glVertexAttribPointer(posBinding, 3, GL_FLOAT, GL_FALSE, (3 + 2 + 3) * 4, reinterpret_cast<void*>(0ULL));
		glVertexAttribPointer(texBinding, 2, GL_FLOAT, GL_FALSE, (3 + 2 + 3) * 4, reinterpret_cast<void*>(12ULL));
		glVertexAttribPointer(nmlBinding, 3, GL_FLOAT, GL_FALSE, (3 + 2 + 3) * 4, reinterpret_cast<void*>(20ULL));

		glVertexBindingDivisor(posBinding, 0);
		glVertexBindingDivisor(texBinding, 0);
		glVertexBindingDivisor(nmlBinding, 0);

		this->modelBuffer.Unbind();
	}

	this->viewportBinding = this->shaderProgram.GetUniformLocation("uViewport");
	this->cameraPosBinding = this->shaderProgram.GetUniformLocation("cameraPos");

	this->ambTexBinding = this->shaderProgram.GetUniformLocation("ambTex");
	this->difTexBinding = this->shaderProgram.GetUniformLocation("difTex");
	this->spcTexBinding = this->shaderProgram.GetUniformLocation("spcTex");
	this->alpTexBinding = this->shaderProgram.GetUniformLocation("alpTex");

	this->dirLightDirBinding = this->shaderProgram.GetUniformLocation("dirLightPos");
	this->dirLightColBinding = this->shaderProgram.GetUniformLocation("dirLightCol");
	this->dirLightIntBinding = this->shaderProgram.GetUniformLocation("dirLightInt");

	this->ambLightColBinding = this->shaderProgram.GetUniformLocation("ambLightCol");
	this->ambLightIntBinding = this->shaderProgram.GetUniformLocation("ambLightInt");

	this->ambBinding = this->shaderProgram.GetUniformLocation("ambMaterialCol");
	this->difBinding = this->shaderProgram.GetUniformLocation("difMaterialCol");
	this->spcBinding = this->shaderProgram.GetUniformLocation("spcMaterialCol");
	this->expBinding = this->shaderProgram.GetUniformLocation("spcMaterialExp");
	this->alpBinding = this->shaderProgram.GetUniformLocation("materialAlp");

	glBindVertexArray(0);
}

ModelVAO::~ModelVAO()
{
	glDeleteVertexArrays(1, &this->handle);
}

void ModelVAO::Bind()
{
	glBindVertexArray(this->handle);
	this->shaderProgram.MakeUsed();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void ModelVAO::Unbind()
{
	glBindVertexArray(0);
	this->shaderProgram.MakeUnused();
}