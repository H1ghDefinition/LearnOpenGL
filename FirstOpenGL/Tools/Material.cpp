#include "Material.h"

Material::Material(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess) :
	shader(_shader),ambient(_ambient),diffuse(_diffuse),specular(_specular),shininess(_shininess)
{

}

Material::~Material()
{

}