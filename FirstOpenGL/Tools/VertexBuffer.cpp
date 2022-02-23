#include "VertexBuffer.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLDebug(glGenBuffers(1, &objectID));
	GLDebug(glBindBuffer(GL_ARRAY_BUFFER, objectID));
	GLDebug(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLDebug(glDeleteBuffers(1, &objectID));
}

void VertexBuffer::Bind() const
{
	GLDebug(glBindBuffer(GL_ARRAY_BUFFER, objectID));
}

void VertexBuffer::Unbind() const
{
	GLDebug(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

