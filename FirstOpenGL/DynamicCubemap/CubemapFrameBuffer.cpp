#include "CubemapFrameBuffer.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

CubemapFrameBuffer::CubemapFrameBuffer(unsigned int unit) : textureUnit(unit)
{
	Init(unit);
}


CubemapFrameBuffer::~CubemapFrameBuffer()
{
	GLDebug(glDeleteRenderbuffers(1, &renderObjectID));
	GLDebug(glDeleteTextures(1, &textureObjectID));
	GLDebug(glDeleteFramebuffers(1, &bufferObjectID));
}

void CubemapFrameBuffer::Init(unsigned int unit)
{
	GLDebug(glGenFramebuffers(1, &bufferObjectID));
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, bufferObjectID));
	GLDebug(glGenTextures(1, &textureObjectID));
	GLDebug(glActiveTexture(GL_TEXTURE0 + unit));
	GLDebug(glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID));
	for (int i = 0; i < 6; i++)
	{
		GLDebug(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLDebug(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	GLDebug(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));

	GLDebug(glGenRenderbuffers(1, &renderObjectID));
	GLDebug(glBindRenderbuffer(GL_RENDERBUFFER, renderObjectID));
	GLDebug(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2048, 2048));
	GLDebug(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	GLDebug(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderObjectID));
	GLDebug(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, textureObjectID, 0));

	CheckFrameBuffer();
}

void CubemapFrameBuffer::CheckFrameBuffer()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}

void CubemapFrameBuffer::Bind()
{
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, bufferObjectID));

	GLDebug(glActiveTexture(GL_TEXTURE0 + textureUnit));
	GLDebug(glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID));

	GLDebug(glViewport(0, 0, 2048, 2048));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void CubemapFrameBuffer::Unbind(int width, int height)
{
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GLDebug(glActiveTexture(GL_TEXTURE0 + textureUnit));
	GLDebug(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

	GLDebug(glViewport(0, 0, width, height));
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void CubemapFrameBuffer::DrawSide(glm::mat4& view, glm::mat4& projection, int side)
{
	// 错误：还未切换到下一面即清空缓冲位，使得当前帧缓冲纹理附件上的颜色缓冲被清空，留下的自然只剩最后一次循环所画上的
	//GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLDebug(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, textureObjectID, 0));
	//CheckFrameBuffer();

	// Important: 将帧缓冲所绑定的纹理附件切换到立方体下一面后再清空缓冲位！！
	GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// 修改观察矩阵和投影矩阵
	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f); // 近远平面取值存疑

	switch (GL_TEXTURE_CUBE_MAP_POSITIVE_X + side) {
	case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
		view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(0, -1, 0));
		break;
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
		view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-10, 0, 0), glm::vec3(0, -1, 0));
		break;
	case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
		view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(0, 0, 1));
		break;
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
		view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -10, 0), glm::vec3(0, 0, -1));
		break;
	case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
		view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10), glm::vec3(0, -1, 0));
		break;
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
		view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -10), glm::vec3(0, -1, 0));
		break;
	default:
		break;
	}
}

