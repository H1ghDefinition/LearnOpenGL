#include "UniformBuffer.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

UniformBuffer::UniformBuffer(int size, unsigned int index, const void* data) : bufferSize(size), bindingIndex(index)
{
	GLDebug(glGenBuffers(1, &objectID));
	GLDebug(glBindBuffer(GL_UNIFORM_BUFFER, objectID));
	GLDebug(glBufferData(GL_UNIFORM_BUFFER, bufferSize, data, GL_STATIC_DRAW));
	GLDebug(glBindBufferBase(GL_UNIFORM_BUFFER, index, objectID));
	GLDebug(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

UniformBuffer::~UniformBuffer()
{
	GLDebug(glDeleteBuffers(1, &objectID));
}

void UniformBuffer::Bind()
{
	GLDebug(glBindBuffer(GL_UNIFORM_BUFFER, objectID));
}

void UniformBuffer::Unbind()
{
	GLDebug(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::IndexBind(unsigned int index)
{
	GLDebug(glBindBufferBase(GL_UNIFORM_BUFFER, index, objectID));
}

void UniformBuffer::BufferData(const void* data)
{
	GLDebug(glBindBuffer(GL_UNIFORM_BUFFER, objectID));
	GLDebug(glBufferData(GL_UNIFORM_BUFFER, bufferSize, data, GL_STATIC_DRAW));
	GLDebug(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
