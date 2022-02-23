#pragma once
#include <string>
#include <vector>
// MSVC break function
#define ASSERT(x)  if(!(x)) __debugbreak();

// Debug macro (using glUtil::DebugUtil first)
#define GLDebug(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

namespace glUtil {
	namespace TextureUtil {
		unsigned int LoadTexture(std::string filePath, unsigned int textureSlot);
		unsigned int LoadCubemap(std::string path, std::vector<std::string> faces, unsigned int textureSlot);
	}
	namespace DebugUtil {
		void GLClearError();
		bool GLLogCall(const char* function, const char* file, int line);
	}
}