#pragma once
#include <glad/glad.h>

class IndexBuffer
{
public:
	/// <param name="data">input datas</param>
	/// <param name="size">count of indices</param>
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	void Bind();
	void Unbind();

	inline unsigned int GetCount() const { return count; }
private:
	unsigned int objectID;
	unsigned int count;
};