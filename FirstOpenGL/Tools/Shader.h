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
	// ����ID
	unsigned int ID;
	// ���캯�� ��·����ȡ�ļ����й���
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ���UniformLocation�Ĺ��ߺ���
	GLint getUniformLocation(const std::string& name) const;
	GLint getUniformBlockIndex(const std::string& name) const;
	// ʹ��/����shader����
	void use();
	void disuse() const;
	// uniform���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat3(const std::string& name, float v0, float v1, float v2) const;
	void setFloat4(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;
	void setMVP(const glm::mat4& m, const glm::mat4& v, glm::mat4& p) const;
	// uniform buffer���
	void BindUniformBlock(const std::string& blockName, unsigned int bindingIndex);

	enum Slot
	{
		DIFFUSE,
		SPECULAR,
		EMISSION
	};

private:
	// ά��uniform map
	mutable std::unordered_map<std::string, GLint> uniformLocationMap;
	// ά��uniform block map
	mutable std::unordered_map<std::string, GLint> uniformBlockIndexMap;
};

#endif // SHADER_H