#pragma once
#include <glad/glad.h>
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "glUtil.h"
#include "stb_image.h"

class SkyBox
{
public:
	SkyBox(const std::string& path, unsigned int unit);
	virtual ~SkyBox();
	unsigned int InitCubemap(const std::string& path);
	void SetFilenames(const std::string& posx, const std::string& negx,
		const std::string& posy, const std::string& negy,
		const std::string& posz, const std::string& negz);
	void SetSkyBoxUniformName(const std::string& name);
	void Draw(Shader& shader);
	void Bind();
	void Unbind();
	unsigned int GetTextureID() { return textureObjectID; };
	unsigned int GetTextureUnit() { return textureUnit; };
private:
	std::string filenames[6];
	std::string uniformName;
	unsigned int textureUnit;
	unsigned int textureObjectID;
	VertexArray skyboxVAO;
	VertexBuffer skyboxVBO;
	static const float skyboxVertices[108];
};