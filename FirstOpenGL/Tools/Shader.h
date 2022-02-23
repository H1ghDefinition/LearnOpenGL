#ifndef SHADER_H
#define SHADER_H 

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader
{
public:
	// 生成ID
	unsigned int ID;
	// 构造函数 由路径读取文件进行构造
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 获得UniformLocation的工具函数
	GLint getUniformLocation(const std::string& name) const;
	GLint getUniformBlockIndex(const std::string& name) const;
	// 使用/激活shader程序
	void use();
	void disuse() const;
	// uniform工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat3(const std::string& name, float v0, float v1, float v2) const;
	void setFloat4(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;
	void setMVP(const glm::mat4& m, const glm::mat4& v, glm::mat4& p) const;
	// uniform buffer相关
	void BindUniformBlock(const std::string& blockName, unsigned int bindingIndex);

	enum Slot
	{
		DIFFUSE,
		SPECULAR,
		EMISSION
	};

private:
	// 维护uniform map
	mutable std::unordered_map<std::string, GLint> uniformLocationMap;
	// 维护uniform block map
	mutable std::unordered_map<std::string, GLint> uniformBlockIndexMap;
};

#endif // SHADER_H