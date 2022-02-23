#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	Shader* shader;
	glm::vec3 ambient;
	unsigned int diffuse; // 使用漫反射光照贴图
	//glm::vec3 specular;
	unsigned int specular; // 使用镜面光照贴图
	unsigned int emission; // 放射光贴图
	float shininess;

	Material(Shader* _shader, glm::vec3 _ambient, unsigned int diffuse, unsigned int specular, unsigned int emission, float shininess);
	~Material();
};

class ColorMaterial
{
public:
	Shader* shader;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	ColorMaterial(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess);
	~ColorMaterial();
};