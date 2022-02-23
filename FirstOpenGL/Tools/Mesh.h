#pragma once
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <vector>

enum TextureType { Diffuse, Specular };

// 用于保存模型中顶点数据的结构体
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

// 用于保存模型中纹理数据的结构体
struct Texture {
	unsigned int textureID;
	TextureType type;
	std::string path;
};

class Mesh
{
public:
	// Mesh数据
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