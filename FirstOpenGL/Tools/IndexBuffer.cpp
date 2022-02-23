#include "IndexBuffer.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	this->count = count;
	GLDebug(glGenBuffers(1, &objectID));
	GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectID));
	GLDebug(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLDebug(glDeleteBuffers(1, &objectID));
}

void IndexBuffer::Bind()
{
	GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectID));
}

void IndexBuffer::Unbind()
{
	GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

