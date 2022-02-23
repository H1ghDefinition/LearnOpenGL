#version 330 core

out vec4 FragColor;

uniform vec3 frameColor;

void main()
{
	FragColor = vec4(frameColor, 1.0);
}
