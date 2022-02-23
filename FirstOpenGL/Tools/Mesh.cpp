#include "Mesh.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

Mesh::Mesh(const std::vector<Vertex>& vers, const std::vector<unsigned int>& inds, const std::vector<Texture>& texs)
	: vertices(vers), indices(inds), textures(texs),
	vao(), vbo((const void*)&vertices[0], vertices.size() * sizeof(Vertex)), ibo(&inds[0], inds.size())
{
	VertexArrayLayout layout;
	layout.Push<float>({ 3, 3, 2 });
	vao.AddBuffer(vbo, layout);
}

Mesh::~Mesh()
{
	vao.~VertexArray();
	vbo.~VertexBuffer();
	ibo.~IndexBuffer();
	std::cout << "Mesh destructor" << std::endl;
}

void Mesh::Draw(Shader& shader)
{
	shader.use();
	for (Texture& tex : textures)
	{
		switch (tex.type)
		{
			// 将漫反射纹理存在纹理单元0
			case Diffuse:
				GLDebug(glActiveTexture(GL_TEXTURE0));
				GLDebug(glBindTexture(GL_TEXTURE_2D, tex.textureID));
				shader.setInt("material.diffuse", 0);
				break;
			// 将镜面光纹理存在纹理单元1
			case Specular:
				GLDebug(glActiveTexture(GL_TEXTURE0 + 1));
				GLDebug(glBindTexture(GL_TEXTURE_2D, tex.textureID));
				shader.setInt("material.specular", 1);
				break;
		}
	}
	vao.Bind();

	GLDebug(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));

	vao.Unbind();
}


