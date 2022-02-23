// fragment shader
#version 330 core
out vec4 FragColor;

in vec3 Normal; // 从顶点着色器传入的法向量
in vec3 FragPos; // 从顶点着色器传入的顶点世界空间坐标

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // 摄像机位置，用于表示镜面光照中的观察者位置

uniform int Blinn;

uniform bool specularOpened;

// const float levels = 3.0;

vec3 GetAmbient()
{
    // 用一个常量颜色作为Ambient Lighting环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    return ambient;
}

vec3 GetDiffuse()
{
    vec3 norm = normalize(Normal); // 在计算时我们只关心向量的方向，因此一般所有计算都用单位向量表示
    vec3 lightDir = normalize(lightPos - FragPos); // 从物体到光源的光线方向 = 光源位置 - 片段位置

    float diff = max(dot(norm,lightDir), 0.0); // 通过片段法向量和光线方向向量的点乘结果大小，计算光源对当前片段实际的漫反射影响

    // 新增Cel Shading
    // float level = floor(diff * levels);
    // diff = level / levels;

    vec3 diffuse = diff * lightColor * objectColor;
    return diffuse;
}

vec3 GetSpecular()
{
    // 未开启镜面光照
    if(!specularOpened)
        return vec3(0.0f);

    vec3 norm = normalize(Normal); // 在计算时我们只关心向量的方向，因此一般所有计算都用单位向量表示
    vec3 lightDir = normalize(lightPos - FragPos); // 从物体到光源的光线方向 = 光源位置 - 片段位置
    // 计算镜面反射光照
    float specularStrength = 0.7825;
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

    // 新增Cel Shading
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

    vec3 result = ambient + diffuse + specular; // 光源颜色和物体颜色相乘（叉乘），得到物体所反射的颜色

//    result = objectColor;
    FragColor = vec4(result, 1.0); 
}
