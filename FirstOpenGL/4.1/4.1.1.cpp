#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

// ��Դ��Ϣ
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Blinn-Phong��Phong�л�
bool Blinn = false;
bool BlinnKeyPressed = false;

// ���ھ۽�״̬
bool isMouseFocus = true;

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
	//�������ߣ�������������һ��GLFWwindow���ʵ����window
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

	Shader cubeShader("./2.1/vert211.vert", "./2.1/frag211.frag");
	Shader frameShader("./4.1/frameshader.vert", "./4.1/frameshader.frag");
	Shader lightShader("./2.1/lt_vert211.vert", "./2.1/lt_frag211.frag");

#pragma region ��������
	// �������� ������Ϊ��ֱ�ڶ������ķ����������ڶ��㱾��û�б��棬���һ��Ҫ������Χ������ɱ�����в�˼��㣬�˴�Ϊ�������壬ֱ���ֹ����ӣ� 
	float vertices[] = {
		//-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		// 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		// 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		// 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		//-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		//-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		// 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		// 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		// 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		//-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		//-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		//-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		//-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		//-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		//-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		// 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		// 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		// 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		// 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		// 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		// 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		//-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		// 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		// 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		// 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		//-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		//-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		//-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		// 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		// 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		// 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		//-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		//-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

	float normals[] = {
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,

		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

		 -1.0f,  0.0f,  0.0f,
		 -1.0f,  0.0f,  0.0f,
		 -1.0f,  0.0f,  0.0f,
		 -1.0f,  0.0f,  0.0f,
		 -1.0f,  0.0f,  0.0f,
		 -1.0f,  0.0f,  0.0f,

		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,

		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,

		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f
	};

	float floor_vertices[] = {
	-15.0f, -3.0f, -15.0f,  0.0f, 1.0f, 0.0f,
	 15.0f, -3.0f, -15.0f,  0.0f, 1.0f, 0.0f,
	 15.0f, -3.0f,  15.0f,  0.0f, 1.0f, 0.0f,
	 15.0f, -3.0f,  15.0f,  0.0f, 1.0f, 0.0f,
	-15.0f, -3.0f,  15.0f,  0.0f, 1.0f, 0.0f,
	-15.0f, -3.0f, -15.0f,  0.0f, 1.0f, 0.0f,
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

#pragma region �������
	{
		VertexArrayLayout layout;
		layout.Push<float>({ 3 });
		VertexArray boxVAO;
		VertexBuffer verticesVBO(vertices, sizeof(vertices));
		VertexBuffer normalsVBO(normals, sizeof(normals));
		boxVAO.AddBuffer(verticesVBO, layout);
		boxVAO.AddBuffer(normalsVBO, layout);

		VertexArray lightVAO;
		lightVAO.AddBuffer(verticesVBO, layout);

		VertexArray floorVAO;
		VertexBuffer floorVBO(floor_vertices, sizeof(floor_vertices));
		layout.Push<float>({ 3 });
		floorVAO.AddBuffer(floorVBO, layout);

		GLDebug(glBindVertexArray(0));

#pragma endregion VAO��VBO��EBO

#pragma region ��Ⱦ

		glEnable(GL_DEPTH_TEST); // ������Ȳ���
		//glDepthFunc(GL_ALWAYS);
		glEnable(GL_STENCIL_TEST); // ����ģ�����
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		ImVec4 objectColor(1.0f, 0.5f, 0.31f, 0.0f);
		ImVec4 lightColor(1.0f, 1.0f, 1.0f, 0.0f);
		ImVec4 floorColor(0.5f, 0.5f, 0.5f, 0.0f);
		ImVec4 frameColor(0.5f, 0.5f, 0.5f, 0.0f);
		bool isSpecularOpened = true;

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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// ��������

			UInewFrame();

			{
				ImGui::Begin("OpenGL Demo UI");
				ImGui::ColorEdit3("Object Color", (float*)&objectColor);
				ImGui::ColorEdit3("Light Color", (float*)&lightColor);
				ImGui::ColorEdit3("Floor Color", (float*)&floorColor);
				ImGui::ColorEdit3("Frame Color", (float*)&frameColor);
				ImGui::Checkbox("Blinn-Phong ilumination model", &Blinn);
				ImGui::Checkbox("Open specular highlights", &isSpecularOpened);
				ImGui::End();
			}

			// ������ɫ��
			cubeShader.use();
			cubeShader.setFloat3("objectColor", objectColor.x, objectColor.y, objectColor.z);
			cubeShader.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);
			cubeShader.setVec3("lightPos", lightPos); // lightPos
			cubeShader.setVec3("viewPos", camera.Position);
			cubeShader.setInt("Blinn", Blinn);
			cubeShader.setBool("specularOpened", isSpecularOpened);
			// ��������
			glm::mat4 model = glm::mat4(1.0f);
			cubeShader.setMat4("model", model);
			glm::mat4 view = camera.GetViewMatrix();
			cubeShader.setMat4("view", view);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			cubeShader.setMat4("projection", projection);

			glStencilMask(0x00); // ����ģ��д�룬ȷ�����Ƶ���ʱ�������ģ�建��
			// ���Ƶ���
			floorVAO.Bind();
			cubeShader.setFloat3("objectColor", floorColor.x, floorColor.y, floorColor.z);
			glm::mat4 floorModel = glm::mat4(1.0f);
			cubeShader.setMat4("model", floorModel);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 6));

			glStencilFunc(GL_ALWAYS, 1, 0xFF); // ����ƬԪʱģ��������ǳɹ�
			glStencilMask(0xFF);
			// ��Ⱦ����
			boxVAO.Bind();
			cubeShader.setFloat3("objectColor", objectColor.x, objectColor.y, objectColor.z);
			model = glm::translate(model, cubePositions[0]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			model = glm::translate(model, cubePositions[2]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			glStencilMask(0x00); // ����ģ��д��
			// �����Դ��ɫ��
			lightShader.use();
			// ���ù�Դ����
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			lightShader.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);
			// ��Ⱦ��Դ
			lightVAO.Bind();
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // ����ģ�建���и�ƬԪֵ������1ʱģ����Գɹ�
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			frameShader.use();
			frameShader.setFloat3("frameColor", frameColor.x, frameColor.y, frameColor.z);
			model = glm::mat4(1.0f);
			frameShader.setMat4("model", model);
			view = camera.GetViewMatrix();
			frameShader.setMat4("view", view);
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			frameShader.setMat4("projection", projection);
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(1.03f));
			model = glm::translate(model, cubePositions[0]);
			frameShader.setMat4("model", model);
			boxVAO.Bind();
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[2]);
			model = glm::scale(model, glm::vec3(1.03f));
			frameShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);

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


	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !BlinnKeyPressed)
	{
		Blinn = !Blinn;
		if (Blinn)
			std::cout << "Now:Phong" << std::endl;
		else
			std::cout << "Now:Blinn-Phong" << std::endl;
		BlinnKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		BlinnKeyPressed = false;
	}

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

			// ���ù�Դ
			std::cout << "Light Placed" << std::endl;
			lightPos = camera.Position;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		isMouseFocus = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}