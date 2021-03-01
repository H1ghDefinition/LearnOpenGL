// fragment shader
#version 330 core
out vec4 FragColor;

in vec3 Normal; // �Ӷ�����ɫ������ķ�����
in vec3 FragPos; // �Ӷ�����ɫ������Ķ�������ռ�����

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // �����λ�ã����ڱ�ʾ��������еĹ۲���λ��

uniform int Blinn;

void main()
{
    // ��һ��������ɫ��ΪAmbient Lighting��������
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // �������������
    vec3 norm = normalize(Normal); // �ڼ���ʱ����ֻ���������ķ������һ�����м��㶼�õ�λ������ʾ
    vec3 lightDir = normalize(lightPos - FragPos); // �����嵽��Դ�Ĺ��߷��� = ��Դλ�� - Ƭ��λ��
    float diff = max(dot(norm,lightDir), 0.0); // ͨ��Ƭ�η������͹��߷��������ĵ�˽����С�������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
    vec3 diffuse = diff * lightColor;

    // ���㾵�淴�����
    float specularStrength = 0.5;
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
    vec3 specular = specularStrength * spec * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor; // ��Դ��ɫ��������ɫ��ˣ���ˣ����õ��������������ɫ
    FragColor = vec4(result, 1.0); 
}