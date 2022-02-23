#include "Shader.h"
#include "glUtil.h"

using namespace glUtil::DebugUtil;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)//������ɫ����Ƭ����ɫ�����ļ���
{
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try // �쳣���
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		// ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. ������ɫ��
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];
	// ������ɫ��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLDebug(glShaderSource(vertexShader, 1, &vShaderCode, NULL));
	GLDebug(glCompileShader(vertexShader));
	GLDebug(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLDebug(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	// Ƭ����ɫ��
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLDebug(glShaderSource(fragmentShader, 1, &fShaderCode, NULL));
	GLDebug(glCompileShader(fragmentShader));
	GLDebug(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLDebug(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	//��ɫ������
	ID = glCreateProgram();
	GLDebug(glAttachShader(ID, vertexShader));
	GLDebug(glAttachShader(ID, fragmentShader));
	GLDebug(glLinkProgram(ID));
	GLDebug(glGetProgramiv(ID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLDebug(glGetProgramInfoLog(ID, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// ɾ����ɫ��
	GLDebug(glDeleteShader(vertexShader));
	GLDebug(glDeleteShader(fragmentShader));
}

void Shader::use()
{
	GLDebug(glUseProgram(ID));
}

void Shader::disuse() const 
{
	GLDebug(glUseProgram(0));
}

void Shader::setBool(const std::string& name, bool value) const
{
	GLDebug(glUniform1i(getUniformLocation(name), (int)value));
}
void Shader::setInt(const std::string& name, int value) const
{
	GLDebug(glUniform1i(getUniformLocation(name), value));
}
void Shader::setFloat(const std::string& name, float value) const
{
	GLDebug(glUniform1f(getUniformLocation(name), value));
}
void Shader::setFloat3(const std::string& name, float v0, float v1, float v2) const
{
	GLDebug(glUniform3f(getUniformLocation(name), v0, v1, v2));
}
void Shader::setFloat4(const std::string& name, float v0, float v1, float v2, float v3) const
{
	GLDebug(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	GLDebug(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setVec3(const std::string& name, const glm::vec3& v) const
{
	GLDebug(glUniform3f(getUniformLocation(name), v.r, v.g, v.b));
}

void Shader::setMVP(const glm::mat4& m, const glm::mat4& v, glm::mat4& p) const
{
	GLDebug(glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, &m[0][0]));
	GLDebug(glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, &v[0][0]));
	GLDebug(glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, &p[0][0]));
}

void Shader::BindUniformBlock(const std::string& blockName, unsigned int bindingIndex)
{
	GLDebug(glUniformBlockBinding(ID, getUniformBlockIndex(blockName), bindingIndex));
}

GLint Shader::getUniformLocation(const std::string& name) const
{
	// ���map�����У�ֱ�ӷ���
	if (uniformLocationMap.count(name))
	{
		return uniformLocationMap[name];
	}
	else
	{
		GLint loc = glGetUniformLocation(ID, name.c_str());
		// ���û���ҵ�
		if (loc != -1)
		{
			uniformLocationMap[name] = loc;
		}
		return loc;
	}
}

GLint Shader::getUniformBlockIndex(const std::string& name) const
{
	if (uniformBlockIndexMap.count(name))
	{
		return uniformBlockIndexMap[name];
	}
	else
	{
		GLint index = glGetUniformBlockIndex(ID, name.c_str());
		if (index != -1)
		{
			uniformBlockIndexMap[name] = index;
		}
		return index;
	}
}
