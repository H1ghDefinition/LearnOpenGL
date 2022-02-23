// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // ������
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal; // ���������ݴ�����Ƭ����ɫ��
out vec3 FragPos; // ����ռ��еĶ�������
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ע��˷�Ҫ���������
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos,1.0)); // ͨ��������λ�����Գ���ģ�;������任������ռ�����
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = vec2(aTexCoords.x,aTexCoords.y);
}