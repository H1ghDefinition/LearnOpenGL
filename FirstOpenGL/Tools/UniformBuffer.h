#pragma once
#include <glad/glad.h>

/// <summary>
/// ��װδ����
/// </summary>
class UniformBuffer
{
public:
	/// <summary>
	/// Uniform�������(�������������Ҫ�����ڴ���룬�������ʼ�����ݣ����Ǻ�����glBufferSubData���)
	/// </summary>
	/// <param name="size">������С</param>
	/// <param name="index">�󶨵�λ��</param>
	/// <param name="data">����</param>
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