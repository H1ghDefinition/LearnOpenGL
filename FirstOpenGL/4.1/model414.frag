// fragment shader
#version 330 core
out vec4 FragColor;

in vec3 Normal; // �Ӷ�����ɫ������ķ�����
in vec3 FragPos; // �Ӷ�����ɫ������Ķ�������ռ�����
in vec2 TexCoords;

uniform vec3 viewPos; // �����λ�ã����ڱ�ʾ��������еĹ۲���λ��

struct Material {
    sampler2D diffuse; // ʹ�������������ͼ
    sampler2D specular; // ʹ�þ������ͼ
    float shininess;
}; // ������ÿ�����շ�������һ����ɫ����

uniform Material material;

uniform samplerCube skybox;

uniform float refractiveIndex;

// 0: reflect 1: refract
uniform int option;

vec4 ReflectEffect()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    return vec4(texture(skybox, R).rgb, 1.0);
}

vec4 RefractEffect()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), 1.0f / refractiveIndex);
    return vec4(texture(skybox, R).rgb, 1.0);
}

void main()
{
//    FragColor = vec4(CalculatePointLighting(light), 1.0);
    switch(option)
    {
        case 0:
            FragColor = ReflectEffect();
            break;
        case 1:
            FragColor = RefractEffect();
    }
}
