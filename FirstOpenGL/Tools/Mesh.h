#pragma once
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <vector>

enum TextureType { Diffuse, Specular };

// ���ڱ���ģ���ж������ݵĽṹ��
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

// ���ڱ���ģ�����������ݵĽṹ��
struct Texture {
	unsigned int textureID;
	TextureType type;
	std::string path;
};

class Mesh
{
public:
	// Mesh����
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(const std::vector<Vertex>& vers, const std::vector<unsigned int>& inds, const std::vector<Texture>& texs);
	~Mesh();
	void Draw(Shader& shader);
private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
};