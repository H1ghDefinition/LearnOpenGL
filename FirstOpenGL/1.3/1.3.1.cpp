#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Tools/Shader.h"

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
		// ���ɴ��������������Ϣ
		std::cout << "Failed to create GLFW window" << std::endl;
		// �ͷ�/ɾ�������������Դ
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ʹ��Shader��
	Shader RectangleShader("./1.3/vertexSource.txt", "./1.3/fragmentSource.txt");

	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 0�ŵ�
		0.5f, -0.5f, 0.0f,  // 1�ŵ�
		-0.5f, -0.5f, 0.0f, // 2�ŵ�
		-0.5f, 0.5f, 0.0f   // 3�ŵ�
	};

	unsigned int indices[] = {
		0, 1, 3, // ��һ��������
		1, 2, 3 // �ڶ���������
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// 1. �󶨶����������
	glBindVertexArray(VAO);
	// 2. �Ѷ������鸴�Ƶ�һ�����㻺����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. �������ǵ��������鵽һ������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. �趨��������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// �߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(238 / (255.0f), 232 / (255.0f), 170 / (255.0f), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		RectangleShader.use();

		float timeValue = glfwGetTime(); // ��ȡ���е�����
		float greenValue = (sin(timeValue) + 1) / 2.0f; // ����ɫ��0.0��1.0֮��ı�
		int vertexColorLocation = glGetUniformLocation(RectangleShader.ID, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteFramebuffers(1, &EBO);
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