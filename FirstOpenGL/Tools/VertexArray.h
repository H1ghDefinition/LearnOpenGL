#pragma once
#include "glad/glad.h"
#include <iostream>
#include <vector>
#include "VertexBuffer.h"

struct VertexArrayLayoutElement {
	unsigned int type;
	unsigned int count;
	GLboolean normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
		}
		std::cout << "TYPE_ERROR" << std::endl;
		return 0;
	}
};

class VertexArrayLayout
{
private:
	std::vector<VertexArrayLayoutElement> elements;
	unsigned int stride;
public:
	VertexArrayLayout() : stride(0) {}
	~VertexArrayLayout() {}

	template<typename T>
	void Push(std::initializer_list<unsigned int> counts)
	{
		static_assert(false, "VertexArrayLayoutElement TYPE_ERROR");
	}

	template<>
	void Push<float>(std::initializer_list<unsigned int> counts)
	{
		for (unsigned int count : counts)
		{
			elements.push_back({ GL_FLOAT, count, GL_FALSE });
			stride += count * sizeof(float);
		}
	}

	template<>
	void Push<unsigned int>(std::initializer_list<unsigned int> counts)
	{
		for (unsigned int count : counts)
		{
			elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			stride += count * sizeof(unsigned int);
		}
	}

	void Reset() { elements.clear(); stride = 0; }

	inline const std::vector<VertexArrayLayoutElement>& GetElements() const { return elements; }
	inline unsigned int GetStride() const { return stride; }
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbo, const VertexArrayLayout& layout);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int objectID;
	unsigned int attribPointerPos; // 记录当前已配置的属性指针位置
};
