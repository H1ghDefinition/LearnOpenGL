#ifndef SHADER_H
#define SHADER_H 

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// 生成ID
	unsigned int ID;
	// 构造函数 由路径读取文件进行构造
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用/激活shader程序
	void use();
	// uniform工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat3(const std::string& name, float v0, float v1, float v2) const;
	void setFloat4(const std::string& name, float v0, float v1, float v2, float v3) const;
};

#endif // SHADER_H