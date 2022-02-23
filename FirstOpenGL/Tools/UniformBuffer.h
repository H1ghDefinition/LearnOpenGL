#pragma once
#include <glad/glad.h>

/// <summary>
/// 封装未测试
/// </summary>
class UniformBuffer
{
public:
	/// <summary>
	/// Uniform缓冲对象(由于添加数据需要考虑内存对齐，不建议初始化数据，而是后续用glBufferSubData添加)
	/// </summary>
	/// <param name="size">缓冲块大小</param>
	/// <param name="index">绑定点位置</param>
	/// <param name="data">数据</param>
	UniformBuffer(int size, unsigned int index, const void* data = nullptr);
	~UniformBuffer();
	void Bind();
	void Unbind();
	void IndexBind(unsigned int index);
	void BufferData(const void* data);
	unsigned int GetID() { return objectID; };
	unsigned int GetBindingIndex() { return bindingIndex; };
private:
	unsigned int objectID;
	unsigned int bindingIndex;
	int bufferSize;
};