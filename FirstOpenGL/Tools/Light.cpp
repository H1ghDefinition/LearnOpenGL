#include "Light.h"

Light::Light(Shader* _shader, glm::vec3 _position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) :
	shader(_shader), position(_position), ambient(_ambient), diffuse(_diffuse), specular(_specular)
{

}

Light::~Light()
{

}