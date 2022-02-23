#include "Light.h"

PointLight::PointLight(Shader* _shader, glm::vec3 _position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular)
	: shader(_shader), position(_position), ambient(_ambient), diffuse(_diffuse), specular(_specular)
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
}

PointLight::~PointLight()
{

}

void PointLight::setAttenuationParams(float c, float l, float q)
{
	constant = c;
	linear = l;
	quadratic = q;
}

DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) 
	: direction(_direction), ambient(_ambient), diffuse(_diffuse), specular(_specular)
{

}

DirectionalLight::~DirectionalLight()
{

}

SpotLight::SpotLight(glm::vec3 _positon, glm::vec3 _direction, float _cutOff, float _outerCutOff, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular)
	: position(_positon), direction(_direction), cutOff(_cutOff), outerCutOff(_outerCutOff), ambient(_ambient), diffuse(_diffuse), specular(_specular)
{

}

SpotLight::~SpotLight()
{

}
