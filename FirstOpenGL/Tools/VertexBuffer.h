#pragma once
#include <glad/glad.h>

class VertexBuffer
{
public:
	/// <param name="data">input datas</param>
	/// <param name="size">size of byte</param>
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
private:
	unsigned int objectID;
};