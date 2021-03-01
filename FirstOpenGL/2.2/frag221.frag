// fragment shader
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; // ������ÿ�����շ�������һ����ɫ����

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal; // �Ӷ�����ɫ������ķ�����
in vec3 FragPos; // �Ӷ�����ɫ������Ķ�������ռ�����

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos; // �����λ�ã����ڱ�ʾ��������еĹ۲���λ��

uniform int Blinn;

uniform Material material;
uniform Light light;

void main()
{
    // ��һ��������ɫ��ΪAmbient Lighting��������
    vec3 ambient = light.ambient * material.ambient;

    // �������������
    vec3 norm = normalize(Normal); // �ڼ���ʱ����ֻ���������ķ������һ�����м��㶼�õ�λ������ʾ
    vec3 lightDir = normalize(light.position - FragPos); // �����嵽��Դ�Ĺ��߷��� = ��Դλ�� - Ƭ��λ��
    float diff = max(dot(norm,lightDir), 0.0); // ͨ��Ƭ�η������͹��߷��������ĵ�˽����С�������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // ���㾵�淴�����
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;
    if(0 == Blinn)
    {
        vec3 reflectDir = reflect(-lightDir, norm); // ������������һ������Ҫ��ӹ�Դָ��Ƭ��λ�õ��������ڶ�������Ҫ��Ƭ�η�����
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // ȡ��˵�32������Ϊ�ù۲�Ƕ��ϵľ��淴��Ӱ�죬���32�Ǹ߹�ķ���ȣ������Խ�ߣ�����������Խǿ��ɢ��Խ�٣��߹��ԽС
    }
    else
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(halfwayDir,norm), 0.0), 64);
    }
    vec3 specular = light.specular * (spec * material.specular);
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = ambient + diffuse + specular; // ��Ϊ�ò������Ա�ʾ�����³��ֵ���ɫ������Ҫ�ٳ������屾����ɫ
    FragColor = vec4(result, 1.0); 
}