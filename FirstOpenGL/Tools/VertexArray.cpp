#pragma once
#include "VertexArray.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

VertexArray::VertexArray() : attribPointerPos(0)
{
	GLDebug(glGenVertexArrays(1, &objectID));
	GLDebug(glBindVertexArray(objectID));
}

VertexArray::~VertexArray()
{
	GLDebug(glDeleteVertexArrays(1, &objectID));
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexArrayLayout& layout)
{
	Bind();
	vbo.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i, ++attribPointerPos)
	{
		const auto& element = elements[i];
		GLDebug(glVertexAttribPointer(attribPointerPos, element.count, element.type, element.normalized,
			layout.GetStride(), (const void*)offset));
		GLDebug(glEnableVertexAttribArray(attribPointerPos));
		offset += element.count * element.GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLDebug(glBindVertexArray(objectID));
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
