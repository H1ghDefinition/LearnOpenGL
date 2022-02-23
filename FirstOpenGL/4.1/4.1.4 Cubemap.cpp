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

OBJModelPath nanosuit{ "./Models/nanosuit/nanosuit.obj" };

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

int shaderOption = 0;

// 加载立方体贴图

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

	float skyboxVertices[] = {
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

#pragma region 缓存对象
#pragma endregion VAO，VBO，EBO

#pragma region 渲染
	{
		Model myModel(nanosuit.objPath);
		//Model myModel("./DynamicCubemap/resources/sphere.obj");

		Shader shader("./4.1/model414.vert", "./4.1/model414.frag");

		//unsigned int diffuseMap = glUtil::TextureUtil::LoadTexture(guard.diffusePath, 0);

		glm::vec3 bkColor(0.0, 0.0, 0.0);
		float refractiveIndex = 1.52f;

		// 立方体贴图
		std::vector<std::string> faces{
			"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"
		};
		unsigned int cubemapTexture = glUtil::TextureUtil::LoadCubemap("./Image/skybox", faces, 3);
		Shader skyboxShader("./4.1/414cubemap.vert", "./4.1/414cubemap.frag");
		VertexArray skyboxVAO;
		VertexBuffer skyboxVBO(skyboxVertices, sizeof(skyboxVertices));
		VertexArrayLayout skyboxLayout;
		skyboxLayout.Push<float>({ 3 });
		skyboxVAO.AddBuffer(skyboxVBO, skyboxLayout);
		skyboxVAO.Unbind();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL); // 调整成小于等于深度缓冲通过测试 使得深度值为1.0的天空盒在最后渲染可以通过深度测试而不会发生z-fighting
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
				ImGui::DragFloat("refractiveIndex", &refractiveIndex, 0.02f, 1.0f, 10.0f);
				const char* items[] = { "reflect", "refract" };
				static int item_current_index = 0;
				const char* combo_preview_value = items[item_current_index];
				if (ImGui::BeginCombo("Effect", combo_preview_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_index == n);
						if (ImGui::Selectable(items[n], is_selected))
						{
							item_current_index = n;
							switch (item_current_index)
							{
							case 0:
								shaderOption = 0;
								break;
							case 1:
								shaderOption = 1;
								break;
							default:
								break;
							}
						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();

				}
				ImGui::End();
			}
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view;
			glm::mat4 projection;

			// 模型绘制
			shader.use();
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			view = camera.GetViewMatrix();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			shader.setMat4("model", model);
			shader.setVec3("viewPos", camera.Position);
			shader.setInt("skybox", 3);
			shader.setFloat("refractiveIndex", refractiveIndex);
			shader.setInt("option", shaderOption);

			//ourModel.Draw(shader);
			myModel.Draw(shader);

			// skybox绘制
			skyboxShader.use();
			skyboxVAO.Bind();
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			skyboxShader.setInt("skybox", 3);
			skyboxShader.setVec3("viewPos", camera.Position);
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("projection", projection);
			glDrawArrays(GL_TRIANGLES, 0, 36);

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