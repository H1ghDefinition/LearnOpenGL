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
	Shader myShader("./1.3/vertexSource_132.txt", "./1.3/fragmentSource_132.txt");

	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// 1. �󶨶����������
	glBindVertexArray(VAO);
	// 2. �Ѷ������鸴�Ƶ�һ�����㻺����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. �趨��������ָ��
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // ����ָ��λ���������˴�Location = 0�������ԣ�ʹ��
	// ��ɫ����                                                        // ��ע��˴���ƫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// �߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ����٤�����
	myShader.setBool("Gamma", IsGammaCorrectionOpen);

	// 4. ������ɫ��
	myShader.use();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);
		// ����Ƿ���٤����� ������ɫ��״̬
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // �Ƿ��·��ؼ�
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // �Ƿ���G��
	{
		IsGammaCorrectionOpen = !IsGammaCorrectionOpen;
	}
}