// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // 法向量

out vec3 Normal; // 法向量数据传出到片段着色器
out vec3 FragPos; // 世界空间中的顶点坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 注意乘法要从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos,1.0)); // 通过将顶点位置属性乘上模型矩阵来变换到世界空间坐标
    Normal = mat3(transpose(inverse(model))) * aNormal;
}