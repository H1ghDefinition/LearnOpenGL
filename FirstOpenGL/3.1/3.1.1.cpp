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
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 窗口聚焦状态
bool isMouseFocus = true;

glm::vec3 lightPos(3.0f, 3.0f, 3.0f);

int main()
{
#pragma region 窗口
	// 实例化GLFW窗口
	glfwInit();//glfw初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 960, "LearnOpenGL", NULL, NULL);
	//（宽，高，窗口名）返回一个GLFWwindow类的实例：window
	if (window == NULL)
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// 告诉GLFW我们希望每当窗口调整大小的时候调用改变窗口大小的函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 配置隐藏光标并捕捉
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	isMouseFocus = true;
	
#pragma endregion GLFW

#pragma region 函数指针
	// glad管理opengl函数指针，初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#pragma endregion GLAD

	// 开启UI
	UIinit(window);

#pragma region 顶点数据
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

#pragma region 缓存对象
#pragma endregion VAO，VBO，EBO

#pragma region 渲染
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
		// 渲染循环
		while (!glfwWindowShouldClose(window))
		{
			// 帧时间差
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// 输入
			processInput(window);

			// 渲染指令
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
			shader.setVec3("light.position", lightPos); // 全局变量lightPos
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
			model = glm::translate(model, lightPos); // 全局变量lightPos
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			lightShader.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);

			lightVAO.Bind();
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			// UI Render
			UIrender();

			// 检查并调用事件，交换缓冲
			glfwSwapBuffers(window);

			// 检查触发什么事件，更新窗口状态
			glfwPollEvents();
		}

		UIclear();
	}

	// 释放之前的分配的所有资源
	glfwTerminate();

	return 0;
}

#pragma endregion Rendering

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// 返回这个按键是否正在被按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//是否按下了返回键
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
			// 放置光源
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