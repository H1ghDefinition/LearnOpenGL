#pragma once
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CubemapFrameBuffer {
public:
	/// <param name="unit">绑定cubemap纹理单元</param>
	CubemapFrameBuffer(unsigned int unit);
	~CubemapFrameBuffer();
	void Init(unsigned int unit);
	void CheckFrameBuffer();
	void Bind();
	void Unbind(int width, int height);
	void DrawSide(glm::mat4& view, glm::mat4& projection, int side);
	unsigned int GetFrameBufferID() { return bufferObjectID; };
	unsigned int GetTextureID() { return textureObjectID; };
private:
	unsigned int bufferObjectID;
	unsigned int textureObjectID;
	unsigned int renderObjectID;
	unsigned int textureUnit;
};