#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight
{
public:
	Shader* shader;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	PointLight(Shader* _shader, glm::vec3 _position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);
	~PointLight();
	
	void setAttenuationParams(float c, float l, float q);
};

class DirectionalLight
{
public:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	DirectionalLight(glm::vec3 _direction, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);
	~DirectionalLight();
};

class SpotLight
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff; // 切光角余弦值
	float outerCutOff; // 外光锥余弦值
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	SpotLight(glm::vec3 _positon, glm::vec3 _direction, float _cutOff, float _outerCutOff, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);
	~SpotLight();
};

