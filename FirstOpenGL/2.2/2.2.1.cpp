#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glUtil.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"

#include "VertexBuffer.h"
#include "VertexArray.h"

#include "UITools.h"

using namespace glUtil;

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
bool isMouseFocus = true;

// timing
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

// ��Դ��Ϣ
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Blinn-Phong��Phong�л�
bool Blinn = false;
bool BlinnKeyPressed = false;

int main()
{
#pragma region ����
	// ʵ����GLFW����
	glfwInit();//glfw��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	glEnable(GL_DEPTH_TEST); // ������Ȳ���
	
	UIinit(window); // ��ʼ��UI

#pragma region ��������
	// �������� ������Ϊ��ֱ�ڶ������ķ����������ڶ��㱾��û�б��棬���һ��Ҫ������Χ������ɱ�����в�˼��㣬�˴�Ϊ�������壬ֱ���ֹ���ӣ� 
	float vertices[] = {
		//position             //normal             //TexCoord 
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,

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
	// ���������ʹ�����������ֲ���������������������Ҫ����Ӧ��װ��VertexArray��VertexBuffer�࣬������ܵ��´��ڹر�ʱ��δ�ͷű�����
	{
		// ��ʼ���������
		//unsigned int VBO;
		//glGenBuffers(1, &VBO);
		//unsigned int VAO;
		//glGenVertexArrays(1, &VAO);
		//// 1. �󶨶����������
		//glBindVertexArray(VAO);
		// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		VertexBuffer vbo(vertices, sizeof(vertices));
		// 3. �趨��������ָ��
		// λ������
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		//// ����������
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		//// ��������
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);
		VertexArrayLayout layout;
		layout.Push<float>({ 3, 3, 2 });
		VertexArray vao;
		vao.AddBuffer(vbo, layout);

		// ��ԴVAO��VBO
		unsigned int lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���Ѿ����úö������ݵ�VBO
		vbo.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ���Զ��������еķ���������
		glEnableVertexAttribArray(0);

		// Shader
		Shader cubeShader("./2.2/vert221.vert", "./2.2/frag221.frag");
		Shader lightShader("./2.2/lt_vert221.vert", "./2.2/lt_frag221.frag");

		PointLight cubeLight(
			&cubeShader,
			lightPos,
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		); // ���ո�������cube������

		DirectionalLight dirLight(glm::vec3(0, -1.0f, 0),
			glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

		SpotLight spotLight(glm::vec3(0.0, 3.0f, 0.0), glm::vec3(0, -1.0f, 0), glm::cos(glm::radians(10.0f)),
			glm::cos(glm::radians(30.0f)), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

		// ����
		unsigned int diffuseMap = TextureUtil::LoadTexture("./Image/box_container.png", 0);
		Material cubeMaterial(
			&cubeShader,
			glm::vec3(1.0f, 0.5f, 0.31f),
			0, // ��������ͼ����Ԫ�������˴�Ϊ��0������Ԫ��
			/*glm::vec3(0.5f, 0.5f, 0.5f),*/
			1, // ���������ͼ����Ԫ�������˴�Ϊ��1������Ԫ��
			2, // �������ͼ
			32.0f
		);
		unsigned int specularMap = TextureUtil::LoadTexture("./Image/box_container_specular.png", 1);
		unsigned int emmisionMap = TextureUtil::LoadTexture("./Image/matrix.jpg", 2);

	#pragma endregion VAO��VBO��EBO

	#pragma region ��Ⱦ

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

			// UI֡
			UInewFrame();
			// UI����
			{
				ImGui::Begin("Test Attenuation");
				ImGui::InputFloat("linear", &cubeLight.linear);
				ImGui::InputFloat("quadratic", &cubeLight.quadratic);
				ImGui::End();
			}

			// ������ɫ��
			cubeShader.use();
			// ����uniform
			cubeShader.setFloat3("objectColor", 1.0f, 0.5f, 0.31f);
			cubeShader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
			cubeShader.setVec3("light.Position", lightPos); // lightPos
			cubeShader.setVec3("viewPos", camera.Position);
			// ���ղ���
			cubeShader.setInt("Blinn", Blinn);
			//cubeShader.setVec3("material.ambient", cubeMaterial.ambient);
			cubeShader.setInt("material.diffuse", cubeMaterial.diffuse);
			cubeShader.setInt("material.specular", cubeMaterial.specular);
			cubeShader.setInt("material.emission", cubeMaterial.emission);
			cubeShader.setFloat("material.shininess", cubeMaterial.shininess);
			cubeShader.setVec3("light.position", lightPos);
			cubeShader.setVec3("light.ambient", cubeLight.ambient);
			cubeShader.setVec3("light.diffuse", cubeLight.diffuse);
			cubeShader.setVec3("light.specular", cubeLight.specular);
			cubeShader.setFloat("time", glfwGetTime());
			cubeShader.setFloat("light.constant", cubeLight.constant);
			cubeShader.setFloat("light.linear", cubeLight.linear);
			cubeShader.setFloat("light.quadratic", cubeLight.quadratic);
			// DirectionLight
			cubeShader.setVec3("dirLight.direction", dirLight.direction);
			cubeShader.setVec3("dirLight.diffuse", dirLight.diffuse);
			cubeShader.setVec3("dirLight.ambient", dirLight.ambient);
			cubeShader.setVec3("dirLight.specular", dirLight.specular);
			// SpotLight
			cubeShader.setVec3("spotLight.diffuse", spotLight.diffuse);
			cubeShader.setVec3("spotLight.ambient", spotLight.ambient);
			cubeShader.setVec3("spotLight.specular", spotLight.specular);
			cubeShader.setVec3("spotLight.position", camera.Position);
			cubeShader.setVec3("spotLight.direction", camera.Front);
			cubeShader.setFloat("spotLight.cutOff", spotLight.cutOff);
			cubeShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
			// ������
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			// ��������
			glm::mat4 model = glm::mat4(1.0f);
			cubeShader.setMat4("model", model);
			glm::mat4 view = camera.GetViewMatrix();
			cubeShader.setMat4("view", view);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			cubeShader.setMat4("projection", projection);
			// ��Ⱦ����
			//glBindVertexArray(VAO);
			vao.Bind();
			// ѭ�����ƶ��������
			for (unsigned int i = 0; i < 10; ++i)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * (i + 1);
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
				cubeShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glDrawArrays(GL_TRIANGLES, 0, 36);
			// ���Ƶ���
			// TODO

			// �����Դ��ɫ��
			lightShader.use();
			// ���ù�Դ����
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			// ��Ⱦ��Դ
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// ��ȾUI
			UIrender();
			// ��鲢�����¼�����������
			glfwSwapBuffers(window);

			// ��鴥��ʲô�¼������´���״̬
			glfwPollEvents();
		}

		// ���UI
		UIclear();

		//glDeleteVertexArrays(1, &VAO);
		glDeleteVertexArrays(1, &lightVAO);
	}
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
