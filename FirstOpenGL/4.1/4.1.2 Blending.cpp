#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

#include "UITools.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glUtil.h"

using namespace glUtil::DebugUtil;

// callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

// ���ھ۽�״̬
bool isMouseFocus = true;

// ��͸����������
void SortTransparencyObject(std::vector<glm::vec3>& objects);

// ģ����ԣ��������
int main()
{
#pragma region ����
	// ʵ����GLFW����
	glfwInit();//glfw��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 960, "LearnOpenGL", NULL, NULL);
	//�����ߣ�������������һ��GLFWwindow���ʵ����window
	if (window == NULL)
	{
		// ���ɴ��������������Ϣ
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// ����GLFW����ϣ��ÿ�����ڵ�����С��ʱ����øı䴰�ڴ�С�ĺ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// �������ع�겢��׽
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	isMouseFocus = true;

	// ����UI
	UIinit(window);

#pragma endregion GLFW

#pragma region ����ָ��
	// glad����opengl����ָ�룬��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// ���ɴ��������������Ϣ
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#pragma endregion GLAD

	Shader cubeShader("./4.1/cube.vert", "./4.1/cube.frag");
	Shader lightShader("./2.1/lt_vert211.vert", "./2.1/lt_frag211.frag");

	unsigned int cubeTex = glUtil::TextureUtil::LoadTexture("./Image/marble.jpg", 0);
	unsigned int floorTex = glUtil::TextureUtil::LoadTexture("./Image/metal.png", 1);
	unsigned int grassTex = glUtil::TextureUtil::LoadTexture("./Image/grass.png", 2);
	unsigned int windowTex = glUtil::TextureUtil::LoadTexture("./Image/blending_transparent_window.png", 3);

#pragma region ��������
	// ��������  
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,

	};

	float UVs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	float floor_vertices[] = {
	-15.0f, -3.0f, -15.0f,  /*0.0f, 1.0f, 0.0f,*/ 0.0f, 0.0f,
	 15.0f, -3.0f, -15.0f,  /*0.0f, 1.0f, 0.0f,*/ 1.0f, 0.0f,
	 15.0f, -3.0f,  15.0f,  /*0.0f, 1.0f, 0.0f,*/ 1.0f, 1.0f,
	 15.0f, -3.0f,  15.0f,  /*0.0f, 1.0f, 0.0f,*/ 1.0f, 1.0f,
	-15.0f, -3.0f,  15.0f,  /*0.0f, 1.0f, 0.0f,*/ 0.0f, 1.0f,
	-15.0f, -3.0f, -15.0f,  /*0.0f, 1.0f, 0.0f,*/ 0.0f, 0.0f
	};

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

#pragma endregion vertices[],indices[]

	glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	std::vector<glm::vec3> windows
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

#pragma region �������
	{
		VertexArrayLayout layout;
		layout.Push<float>({ 3 });
		VertexArray boxVAO;
		VertexBuffer verticesVBO(vertices, sizeof(vertices));
		VertexBuffer uvVBO(UVs, sizeof(UVs));
		boxVAO.AddBuffer(verticesVBO, layout);
		layout.Reset();
		layout.Push<float>({ 2 });
		boxVAO.AddBuffer(uvVBO, layout);

		VertexArray floorVAO;
		VertexBuffer floorVBO(floor_vertices, sizeof(floor_vertices));
		layout.Reset();
		layout.Push<float>({ 3, 2 });
		floorVAO.AddBuffer(floorVBO, layout);

		VertexArray transparencyVAO;
		VertexBuffer transparencyVBO(transparentVertices, sizeof(transparentVertices));
		transparencyVAO.AddBuffer(transparencyVBO, layout);

		GLDebug(glBindVertexArray(0));
		

#pragma endregion VAO��VBO��EBO

#pragma region ��Ⱦ

		glEnable(GL_DEPTH_TEST); // ������Ȳ���
		//glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND); // ���û��
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha * sourceColor + (1 - alpha) * destinationColor
		glEnable(GL_CULL_FACE);

		// ��Ⱦѭ��
		while (!glfwWindowShouldClose(window))
		{
			// ֡ʱ���
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// ����
			processInput(window);

			// ��Ⱦָ��
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// ��������

			UInewFrame();

			{
				ImGui::Begin("OpenGL Demo UI");
				ImGui::End();
			}

			// ������ɫ��
			cubeShader.use();
			// ��������
			glm::mat4 model = glm::mat4(1.0f);
			cubeShader.setMat4("model", model);
			glm::mat4 view = camera.GetViewMatrix();
			cubeShader.setMat4("view", view);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			cubeShader.setMat4("projection", projection);

			// ���Ƶ���
			floorVAO.Bind();
			glm::mat4 floorModel = glm::mat4(1.0f);
			cubeShader.setMat4("model", floorModel);
			cubeShader.setInt("tex", 1);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 6));

			// ��Ⱦ����
			boxVAO.Bind();
			cubeShader.setInt("tex", 0);
			model = glm::translate(model, cubePositions[0]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[2]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			// ��Ⱦ��
			cubeShader.setInt("tex", 2);
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[4]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 6));

			// ��˳�����͸������
			cubeShader.setInt("tex", 3);
			SortTransparencyObject(windows);
			for (auto pos : windows)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, pos);
				cubeShader.setMat4("model", model);
				GLDebug(glDrawArrays(GL_TRIANGLES, 0, 6));
			}

			// UI Render
			UIrender();

			// ��鲢�����¼�����������
			glfwSwapBuffers(window);

			// ��鴥��ʲô�¼������´���״̬
			glfwPollEvents();
		}
	}
	UIclear();

	// �ͷ�֮ǰ�ķ����������Դ
	glfwTerminate();

	return 0;
}

#pragma endregion Rendering

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ÿ�����ڸı��С��GLFW�������������������Ӧ�Ĳ������㴦��
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// ������������Ƿ����ڱ�����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//�Ƿ����˷��ؼ�
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (isMouseFocus)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		firstMouse = true;
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			if (!isMouseFocus)
			{
				isMouseFocus = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				return;
			}
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		isMouseFocus = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void SortTransparencyObject(std::vector<glm::vec3>& objects)
{
	std::map<float, glm::vec3> sorted;
	for (auto pos : objects)
	{
		float distance = glm::distance(camera.Position, pos);
		sorted[distance] = pos;
	}
	int i = 0;
	for (std::map<float, glm::vec3>::reverse_iterator rit = sorted.rbegin(); rit != sorted.rend(); ++rit)
	{
		objects[i++] = rit->second;
	}
}

