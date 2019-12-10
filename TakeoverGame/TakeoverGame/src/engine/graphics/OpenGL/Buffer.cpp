#include "Buffer.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

using namespace vlk;

namespace
{
	UInt GenBuffer()
	{
		UInt buffer = 0;
		glGenBuffers(1, &buffer);
		return buffer;
	}
}

GLBuffer::GLBuffer() :
	handle(GenBuffer())
{
	this->size = 0;
	this->target = 0;
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &this->handle);
}

void GLBuffer::Resize(vlk::Size newSize, UInt usage, const Byte* data)
{
	glBufferData(target, newSize, data, usage);
	this->size = newSize;
}

void GLBuffer::Fill(const Byte* data, vlk::Size length)
{
	glBufferSubData(target, 0, length, data);
}

void GLBuffer::Bind(UInt target)
{
	glBindBuffer(target, this->handle);
	this->target = target;
}

void GLBuffer::Unbind()
{
	glBindBuffer(target, 0);
	this->target = 0;
}