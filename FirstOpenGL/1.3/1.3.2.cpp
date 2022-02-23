#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Tools/Shader.h"

bool IsGammaCorrectionOpen = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Rectangle", NULL, NULL);
	if (window == NULL)
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to create GLFW window" << std::endl;
		// 释放/删除分配的所有资源
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 使用Shader类
	Shader myShader("./1.3/vertexSource_132.txt", "./1.3/fragmentSource_132.txt");

	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// 1. 绑定顶点数组对象
	glBindVertexArray(VAO);
	// 2. 把顶点数组复制到一个顶点缓冲中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 设定顶点属性指针
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // 启用指定位置索引（此处Location = 0）的属性，使其
	// 颜色属性                                                        // ↓注意此处的偏移量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// 线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 开启伽马矫正
	myShader.setBool("Gamma", IsGammaCorrectionOpen);

	// 4. 激活着色器
	myShader.use();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		// 检测是否开启伽马矫正 更新着色器状态
		myShader.setBool("Gamma", IsGammaCorrectionOpen);
		myShader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteFramebuffers(1, &VBO);
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
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // 是否按下G键
	{
		IsGammaCorrectionOpen = !IsGammaCorrectionOpen;
	}
}