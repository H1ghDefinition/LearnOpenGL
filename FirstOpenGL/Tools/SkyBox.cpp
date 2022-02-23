#include "SkyBox.h"

using namespace glUtil::DebugUtil;

// 立方体顶点数据
const float SkyBox::skyboxVertices[108] = {
		-0.5f, -0.5f, -0.5f, // 1
		 0.5f, -0.5f, -0.5f, // 2
		 0.5f,  0.5f, -0.5f, // 3
		 0.5f,  0.5f, -0.5f, // 3
		-0.5f,  0.5f, -0.5f, // 4
		-0.5f, -0.5f, -0.5f, // 1

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
};

SkyBox::SkyBox(const std::string& path, unsigned int unit) : uniformName("skybox"), textureUnit(unit),
	skyboxVBO(skyboxVertices, sizeof(skyboxVertices))
{

	SetFilenames("right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg");
	textureObjectID = InitCubemap(path);
	VertexArrayLayout skyboxLayout;
	skyboxLayout.Push<float>({ 3 });
	skyboxVAO.AddBuffer(skyboxVBO, skyboxLayout);
}

SkyBox::~SkyBox()
{
	GLDebug(glDeleteTextures(1, &textureObjectID));
}

unsigned int SkyBox::InitCubemap(const std::string& path)
{
	unsigned int texture;
	GLDebug(glGenTextures(1, &texture));
	GLDebug(glActiveTexture(GL_TEXTURE0 + textureUnit));
	GLDebug(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));

	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)); // 立方体贴图第三个维度
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLDebug(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, nrChannels;
	for (int i = 0; i < 6; ++i)
	{
		std::string filePath = path + filenames[i];
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = GL_RGB;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3) // 位深度24,3通道（.jpg）
				format = GL_RGB;
			else if (nrChannels == 4) // 位深度32,6通道（.png）
				format = GL_RGBA;

			// positive-x, negative-x, positive-y, negative-y, positive-z, negative-z
			GLDebug(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
				0, format, GL_UNSIGNED_BYTE, data));
		}
		else
		{
			std::cout << "Failed to load texture at " << filePath << std::endl;
		}
		stbi_image_free(data);
	}

	GLDebug(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));	// 生成Mipmap
	GLDebug(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS)); // 相邻两个面采样时使用两个面上的纹素插值

	return texture;
}

void SkyBox::SetFilenames(const std::string& posx, const std::string& negx,
	const std::string& posy, const std::string& negy,
	const std::string& posz, const std::string& negz)
{
	filenames[0] = posx; filenames[1] = negx;
	filenames[2] = posy; filenames[3] = negy;
	filenames[4] = posz; filenames[5] = negz;
}

void SkyBox::SetSkyBoxUniformName(const std::string& name)
{
	uniformName = name;
}

void SkyBox::Draw(Shader& shader)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glDepthMask(GL_FALSE);
	shader.use();
	GLDebug(glActiveTexture(GL_TEXTURE0 + textureUnit));
	GLDebug(glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID));
	shader.setInt(uniformName, textureUnit);
	skyboxVAO.Bind();
	GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
	skyboxVAO.Unbind();
	shader.disuse();
	glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
}

void SkyBox::Bind()
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID);
}

void SkyBox::Unbind()
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

