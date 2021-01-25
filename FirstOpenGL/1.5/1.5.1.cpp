#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// ��ʼ��glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle with movement and color", NULL, NULL);
	if (window == NULL)
	{
		// ���ɴ��������������Ϣ
		std::cout << "Failed to create GLFW window" << std::endl;
		// �ͷ�/ɾ�������������Դ
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ע��ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// ʹ��Shader��
	
	// ��������

	// VAO��VBO��EBO
	unsigned int VAO, VBO, EBO;
	// 1. �󶨶����������

	// 2. �Ѷ������鸴�Ƶ�һ�����㻺����

	// 3. �趨��������ָ��

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// ��Ⱦָ��

		// ��������
		glfwSwapBuffers(window);
		
		// 
		glfwPollEvents();
	}

	// �ͷ�/ɾ�������������Դ
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // �Ƿ��·��ؼ�
		glfwSetWindowShouldClose(window, true);
}