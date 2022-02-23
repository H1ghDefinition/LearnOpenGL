#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Camera.h"

#include "UITools.h"
#include "Model.h"
#include "Light.h"
#include "glUtil.h"
using namespace glUtil::DebugUtil;

struct OBJModelPath {
	std::string objPath;
	std::string diffusePath;
	std::string specularPath;
};

OBJModelPath dummy{"./Models/dummy/dummy.obj", "./Models/dummy/dummy.png" };
OBJModelPath nanosuit{"./Models/nanosuit/nanosuit.obj"};
OBJModelPath mage{"./Models/Mage/Mage.obj", "./Models/Mage/Mage.png" };
OBJModelPath guard{"./Models/Ruin Guard/Ruin Guard.obj", "./Models/Ruin Guard/Ruin Guard.png" };
OBJModelPath paimon{ "./Models/paimon/paimon.obj" };

// callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 960;

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

glm::vec3 lightPos(3.0f, 3.0f, 3.0f);

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

	// ����UI
	UIinit(window);

#pragma region ��������
#pragma endregion vertices[],indices[]
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

#pragma region �������
#pragma endregion VAO��VBO��EBO

#pragma region ��Ⱦ
	{
		//Model myModel(guard.objPath);
		Model myModel("./DynamicCubemap/resources/sphere.obj");

		Shader shader("./3.1/vert311.vert", "./3.1/frag311.frag");

		VertexArray lightVAO;
		VertexArrayLayout lightLayout;
		lightLayout.Push<float>({ 3, 3 });
		VertexBuffer lightVBO(vertices, sizeof(vertices));
		lightVAO.Unbind();
		
		Shader lightShader("./2.1/lt_vert211.vert", "./2.1/lt_frag211.frag");
		glm::vec3 lightColor(0.8f, 0.8f, 0.8f);
		glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
		glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f);
		glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
		PointLight pointLight = PointLight(&lightShader, lightPos, lightAmbient, lightDiffuse, lightSpecular);
		float lightAttenuationTerms[] = { 1.0f, 0.022f, 0.0019f };

		unsigned int diffuseMap = glUtil::TextureUtil::LoadTexture(guard.diffusePath, 0);

		glm::vec3 bkColor(0.0, 0.0, 0.0);

		glEnable(GL_DEPTH_TEST);
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
			GLDebug(glClearColor(bkColor.r, bkColor.g, bkColor.b, 1.0f));
			GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			UInewFrame();
			{
				ImGui::Begin("OpenGL Demo UI");
				ImGui::ColorEdit3("Background Color", (float*)&(bkColor));
				ImGui::DragFloat3("Light Ambient", (float*)&(pointLight.ambient), 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat3("Light Diffuse", (float*)&(pointLight.diffuse), 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat3("Light Specular", (float*)&(pointLight.specular), 0.05f, 0.0f, 1.0f);
				ImGui::InputFloat3("Light Attenuation", lightAttenuationTerms);
				ImGui::End();
			}

			shader.use();

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			shader.setMat4("model", model);
			shader.setVec3("lightColor", lightColor);
			shader.setVec3("viewPos", camera.Position);
			shader.setVec3("light.position", lightPos); // ȫ�ֱ���lightPos
			shader.setVec3("light.ambient", pointLight.ambient);
			shader.setVec3("light.diffuse", pointLight.diffuse);
			shader.setVec3("light.specular", pointLight.specular);
			shader.setFloat("light.constant", lightAttenuationTerms[0]);
			shader.setFloat("light.linear", lightAttenuationTerms[1]);
			shader.setFloat("light.quadratic", lightAttenuationTerms[2]);
			shader.setFloat("material.shininess", 64.0f);

			//ourModel.Draw(shader);

			myModel.Draw(shader);

			lightShader.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos); // ȫ�ֱ���lightPos
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			lightShader.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);

			lightVAO.Bind();
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			// UI Render
			UIrender();

			// ��鲢�����¼�����������
			glfwSwapBuffers(window);

			// ��鴥��ʲô�¼������´���״̬
			glfwPollEvents();
		}

		UIclear();
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
	{		
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{		
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{	
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{		
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{		
		camera.ProcessKeyboard(DOWN, deltaTime);
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