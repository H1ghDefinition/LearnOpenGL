// fragment shader
#version 330 core
out vec4 FragColor;

in vec3 Normal; // 从顶点着色器传入的法向量
in vec3 FragPos; // 从顶点着色器传入的顶点世界空间坐标
in vec2 TexCoords;

uniform vec3 viewPos; // 摄像机位置，用于表示镜面光照中的观察者位置

struct Material {
    sampler2D diffuse; // 使用漫反射光照贴图
    sampler2D specular; // 使用镜面光贴图
    float shininess;
}; // 给材质每个光照分量定义一个颜色向量

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
