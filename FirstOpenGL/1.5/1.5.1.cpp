#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// 初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle with movement and color", NULL, NULL);
	if (window == NULL)
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to create GLFW window" << std::endl;
		// 释放/删除分配的所有资源
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// 使用Shader类
	
	// 顶点数据

	// VAO、VBO、EBO
	unsigned int VAO, VBO, EBO;
	// 1. 绑定顶点数组对象

	// 2. 把顶点数组复制到一个顶点缓冲中

	// 3. 设定顶点属性指针

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		// 渲染指令

		// 交换缓冲
		glfwSwapBuffers(window);
		
		// 
		glfwPollEvents();
	}

	// 释放/删除分配的所有资源
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 是否按下返回键
		glfwSetWindowShouldClose(window, true);
}