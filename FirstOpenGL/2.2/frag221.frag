// fragment shader
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; // 给材质每个光照分量定义一个颜色向量

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal; // 从顶点着色器传入的法向量
in vec3 FragPos; // 从顶点着色器传入的顶点世界空间坐标

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos; // 摄像机位置，用于表示镜面光照中的观察者位置

uniform int Blinn;

uniform Material material;
uniform Light light;

void main()
{
    // 用一个常量颜色作为Ambient Lighting环境光照
    vec3 ambient = light.ambient * material.ambient;

    // 计算漫反射光照
    vec3 norm = normalize(Normal); // 在计算时我们只关心向量的方向，因此一般所有计算都用单位向量表示
    vec3 lightDir = normalize(light.position - FragPos); // 从物体到光源的光线方向 = 光源位置 - 片段位置
    float diff = max(dot(norm,lightDir), 0.0); // 通过片段法向量和光线方向向量的点乘结果大小，计算光源对当前片段实际的漫反射影响
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 计算镜面反射光照
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;
    if(0 == Blinn)
    {
        vec3 reflectDir = reflect(-lightDir, norm); // 反射向量，第一个参数要求从光源指向片段位置的向量，第二个参数要求片段法向量
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 取点乘的32次幂作为该观察角度上的镜面反射影响，这个32是高光的反光度，反光度越高，反射光的能力越强，散射越少，高光点越小
    }
    else
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(halfwayDir,norm), 0.0), 64);
    }
    vec3 specular = light.specular * (spec * material.specular);
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = ambient + diffuse + specular; // 改为用材质属性表示光照下呈现的颜色，不需要再乘上物体本身颜色
    FragColor = vec4(result, 1.0); 
}