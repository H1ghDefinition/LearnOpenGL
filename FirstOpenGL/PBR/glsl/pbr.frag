#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 camPos;
uniform vec3 albedo; // 反射率
uniform float metallic; // 金属度
uniform float roughness; // 粗糙度
uniform float ao; // 环境光遮蔽

// 点光源
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight lights[4];

void main()
{
	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - FragPos);
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i)
    {
        vec3
    }
}