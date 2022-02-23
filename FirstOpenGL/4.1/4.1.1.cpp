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
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 光源信息
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Blinn-Phong和Phong切换
bool Blinn = false;
bool BlinnKeyPressed = false;

// 窗口聚焦状态
bool isMouseFocus = true;

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

	Shader cubeShader("./2.1/vert211.vert", "./2.1/frag211.frag");
	Shader frameShader("./4.1/frameshader.vert", "./4.1/frameshader.frag");
	Shader lightShader("./2.1/lt_vert211.vert", "./2.1/lt_frag211.frag");

#pragma region 顶点数据
	// 顶点数据 后三项为垂直于顶点表面的法向量（由于顶点本身没有表面，因此一般要利用周围顶点组成表面进行叉乘计算，此处为简单立方体，直接手工添加） 
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

#pragma region 缓存对象
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

#pragma endregion VAO，VBO，EBO

#pragma region 渲染

		glEnable(GL_DEPTH_TEST); // 启用深度测试
		//glDepthFunc(GL_ALWAYS);
		glEnable(GL_STENCIL_TEST); // 启用模板测试
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		ImVec4 objectColor(1.0f, 0.5f, 0.31f, 0.0f);
		ImVec4 lightColor(1.0f, 1.0f, 1.0f, 0.0f);
		ImVec4 floorColor(0.5f, 0.5f, 0.5f, 0.0f);
		ImVec4 frameColor(0.5f, 0.5f, 0.5f, 0.0f);
		bool isSpecularOpened = true;

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
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// 创建矩阵

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

			// 激活着色器
			cubeShader.use();
			cubeShader.setFloat3("objectColor", objectColor.x, objectColor.y, objectColor.z);
			cubeShader.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);
			cubeShader.setVec3("lightPos", lightPos); // lightPos
			cubeShader.setVec3("viewPos", camera.Position);
			cubeShader.setInt("Blinn", Blinn);
			cubeShader.setBool("specularOpened", isSpecularOpened);
			// 创建矩阵
			glm::mat4 model = glm::mat4(1.0f);
			cubeShader.setMat4("model", model);
			glm::mat4 view = camera.GetViewMatrix();
			cubeShader.setMat4("view", view);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			cubeShader.setMat4("projection", projection);

			glStencilMask(0x00); // 禁用模板写入，确保绘制地面时不会更新模板缓冲
			// 绘制地面
			floorVAO.Bind();
			cubeShader.setFloat3("objectColor", floorColor.x, floorColor.y, floorColor.z);
			glm::mat4 floorModel = glm::mat4(1.0f);
			cubeShader.setMat4("model", floorModel);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 6));

			glStencilFunc(GL_ALWAYS, 1, 0xFF); // 更新片元时模板测试总是成功
			glStencilMask(0xFF);
			// 渲染箱子
			boxVAO.Bind();
			cubeShader.setFloat3("objectColor", objectColor.x, objectColor.y, objectColor.z);
			model = glm::translate(model, cubePositions[0]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));
			model = glm::translate(model, cubePositions[2]);
			cubeShader.setMat4("model", model);
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			glStencilMask(0x00); // 禁用模板写入
			// 激活光源着色器
			lightShader.use();
			// 设置光源矩阵
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			lightShader.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);
			// 渲染光源
			lightVAO.Bind();
			GLDebug(glDrawArrays(GL_TRIANGLES, 0, 36));

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // 仅在模板缓冲中该片元值不等于1时模板测试成功
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