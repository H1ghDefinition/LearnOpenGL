// fragment shader
#version 330 core
out vec4 FragColor;

in vec3 Normal; // 从顶点着色器传入的法向量
in vec3 FragPos; // 从顶点着色器传入的顶点世界空间坐标
in vec2 TexCoords;

uniform vec3 viewPos; // 摄像机位置，用于表示镜面光照中的观察者位置

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

// 方向光
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};

// 聚光
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};

struct Material {
    sampler2D diffuse; // 使用漫反射光照贴图
    sampler2D specular; // 使用镜面光贴图
    float shininess;
}; // 给材质每个光照分量定义一个颜色向量

uniform Material material;
uniform PointLight light;

vec3 CalculatePointLighting(PointLight light)
{
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(halfwayDir,norm), 0.0), material.shininess * 2);

    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb; 

    float distance = distance(light.position, FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateDirLighting(DirLight light)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // 漫反射着色
    float diff = max(dot(norm, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLighting(SpotLight light)
{
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // 光源到片元方向 与 光照方向 的夹角余弦
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  

    // 漫反射着色
    float diff = max(dot(norm, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, TexCoords));
    return ambient + (diffuse + specular) * intensity;
}

void main()
{
    FragColor = vec4(CalculatePointLighting(light), 1.0);
}
