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
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 窗口聚焦状态
bool isMouseFocus = true;

// 后处理效果
bool invert = false;
bool gray = false;
bool sharpen = false;
bool blur = false;
bool edge = false;

void ResetPostProcessing()
{
	invert = gray = sharpen = blur = edge = false;
}

// 模板测试：物体描边
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

	// 开启UI
	UIinit(window);

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

	Shader cubeShader("./4.1/cube.vert", "./4.1/cube.frag");
	Shader lightShader("./2.1/lt_vert211.vert", "./2.1/lt_frag211.frag");
	Shader screenShader("./4.1/frame.vert", "./4.1/frame.frag");

	unsigned int cubeTex = glUtil::TextureUtil::LoadTexture("./Image/marble.jpg", 0);
	unsigned int floorTex = glUtil::TextureUtil::LoadTexture("./Image/metal.png", 1);

#pragma region 顶点数据
	// 顶点数据  
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

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
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

#pragma region 缓存对象
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

		// 帧缓冲用到的顶点数据
		VertexArray quadVAO;
		VertexBuffer quadVBO(quadVertices, sizeof(quadVertices));
		layout.Reset();
		layout.Push<float>({ 2, 2 });
		quadVAO.AddBuffer(quadVBO, layout);

		GLDebug(glBindVertexArray(0));

		// 帧缓冲
		unsigned int fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// 纹理附件（颜色缓冲）
		unsigned int texColorBuffer;
		glGenTextures(1, &texColorBuffer);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 附加到帧缓冲上
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		// 渲染缓冲对象附件（深度、模板缓冲）不附加深度缓冲会出错
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		// 解绑帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		screenShader.use();
		screenShader.setInt("screenTexture", 2);

#pragma endregion VAO，VBO，EBO

#pragma region 渲染

		glEnable(GL_DEPTH_TEST); // 启用深度测试
		//glDepthFunc(GL_ALWAYS);

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
			// 1st pass
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 创建矩阵

			UInewFrame();

			{
				ImGui::Begin("OpenGL Demo UI");
				// combo
				const char* items[] = { "null", "invert", "gray", "sharpen", "blur", "edge" };
				static int item_current_index = 0;
				const char* combo_preview_value = items[item_current_index];
				if (ImGui::BeginCombo("Post Processing Effects", combo_preview_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_index == n);
						if (ImGui::Selectable(items[n], is_selected))
						{
							item_current_index = n;
							ResetPostProcessing();
							switch (item_current_index)
							{
							case 1:
								invert = true;
								break;
							case 2:
								gray = true;
								break;
							case 3:
								sharpen = true;
								break;
							case 4:
								blur = true;
								break;
							case 5:
								edge = true;
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

			// 激活着色器
			cubeShader.use();
			// 创建矩阵
			glm::mat4 model = glm::mat4(1.0f);
			cubeShader.setMat4("model", model);
			glm::mat4 view = camera.GetViewMatrix();
			cubeShader.setMat4("view", view);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			cubeShader.setMat4("projection", projection);

			// 绘制地面
			floorVAO.Bind();
			glm::mat4 floorModel = glm::mat4(1.0f);
			cubeShader.setMat4("model", floorModel);
			cubeShader.setInt("tex", 1);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 6));

			// 渲染箱子
			boxVAO.Bind();
			cubeShader.setInt("tex", 0);
			model = glm::translate(model, cubePositions[0]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[2]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			
			// 2nd pass
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 线框模式
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			screenShader.use();
			screenShader.setBool("invert", invert);
			screenShader.setBool("gray", gray);
			screenShader.setBool("sharpen", sharpen);
			screenShader.setBool("blur", blur);
			screenShader.setBool("edge", edge);
			quadVAO.Bind();
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			// UI Render
			UIrender();

			// 检查并调用事件，交换缓冲
			glfwSwapBuffers(window);

			// 检查触发什么事件，更新窗口状态
			glfwPollEvents();
		}
	}
	UIclear();

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
