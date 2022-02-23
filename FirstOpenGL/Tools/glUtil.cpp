#include <iostream>
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glUtil.h"

namespace glUtil {
	namespace TextureUtil {

		unsigned int LoadTexture(std::string filePath, unsigned int textureSlot)
		{
			unsigned int texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE0 + textureSlot); // 激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, texture);

			// 为当前绑定的纹理对象设置环绕、过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int width, height, nrChannels;
			//stbi_set_flip_vertically_on_load(true); // 加载图像前设置 进行Y轴翻转
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

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture at " << filePath << std::endl;
			}
			stbi_image_free(data);
			return texture;
		} 

		unsigned int LoadCubemap(std::string path, std::vector<std::string> faces, unsigned int textureSlot)
		{
			unsigned int texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // 立方体贴图第三个维度
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int width, height, nrChannels;
			for (int i = 0; i < faces.size(); ++i)
			{
				std::string filePath = path + "/" + faces[i];
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
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				}
				else
				{
					std::cout << "Failed to load texture at " << filePath << std::endl;
				}
				stbi_image_free(data);
			}
			
			return texture;
		}

	}

	namespace DebugUtil {

		void GLClearError()
		{
			while (glGetError() != GL_NO_ERROR);
		}

		bool GLLogCall(const char* function, const char* file, int line)
		{
			while (GLenum error = glGetError())
			{
				std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
				return false;
			}
			return true;
		}

	}
}
