// fragment shader
#version 330 core
out vec4 FragColor;

in vec3 Normal; // �Ӷ�����ɫ������ķ�����
in vec3 FragPos; // �Ӷ�����ɫ������Ķ�������ռ�����
in vec2 TexCoords;

uniform vec3 viewPos; // �����λ�ã����ڱ�ʾ��������еĹ۲���λ��

// ���Դ
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

// �����
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};

// �۹�
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
    sampler2D diffuse; // ʹ�������������ͼ
    sampler2D specular; // ʹ�þ������ͼ
    float shininess;
}; // ������ÿ�����շ�������һ����ɫ����

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
    // ��������ɫ
    float diff = max(dot(norm, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ����
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
    // ��Դ��ƬԪ���� �� ���շ��� �ļн�����
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  

    // ��������ɫ
    float diff = max(dot(norm, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, TexCoords));
    return ambient + (diffuse + specular) * intensity;
}

void main()
{
    FragColor = vec4(CalculatePointLighting(light), 1.0);
}
