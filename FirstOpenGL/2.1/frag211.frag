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

uniform bool specularOpened;

// const float levels = 3.0;

vec3 GetAmbient()
{
    // ��һ��������ɫ��ΪAmbient Lighting��������
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    return ambient;
}

vec3 GetDiffuse()
{
    vec3 norm = normalize(Normal); // �ڼ���ʱ����ֻ���������ķ������һ�����м��㶼�õ�λ������ʾ
    vec3 lightDir = normalize(lightPos - FragPos); // �����嵽��Դ�Ĺ��߷��� = ��Դλ�� - Ƭ��λ��

    float diff = max(dot(norm,lightDir), 0.0); // ͨ��Ƭ�η������͹��߷��������ĵ�˽����С�������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��

    // ����Cel Shading
    // float level = floor(diff * levels);
    // diff = level / levels;

    vec3 diffuse = diff * lightColor * objectColor;
    return diffuse;
}

vec3 GetSpecular()
{
    // δ�����������
    if(!specularOpened)
        return vec3(0.0f);

    vec3 norm = normalize(Normal); // �ڼ���ʱ����ֻ���������ķ������һ�����м��㶼�õ�λ������ʾ
    vec3 lightDir = normalize(lightPos - FragPos); // �����嵽��Դ�Ĺ��߷��� = ��Դλ�� - Ƭ��λ��
    // ���㾵�淴�����
    float specularStrength = 0.7825;
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

    // ����Cel Shading
    // level = floor(spec * levels);
    // spec = level / levels;

    vec3 specular = specularStrength * spec * lightColor * objectColor;
    return specular;
}

void main()
{
    vec3 ambient = GetAmbient();

    vec3 diffuse = GetDiffuse();

    vec3 specular = GetSpecular();

    vec3 result = ambient + diffuse + specular; // ��Դ��ɫ��������ɫ��ˣ���ˣ����õ��������������ɫ

//    result = objectColor;
    FragColor = vec4(result, 1.0); 
}
