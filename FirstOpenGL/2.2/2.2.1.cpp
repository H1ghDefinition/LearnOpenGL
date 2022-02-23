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
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 光源信息
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Blinn-Phong和Phong切换
bool Blinn = false;
bool BlinnKeyPressed = false;

int main()
{
#pragma region 窗口
	// 实例化GLFW窗口
	glfwInit();//glfw初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	glEnable(GL_DEPTH_TEST); // 启用深度测试
	
	UIinit(window); // 初始化UI

#pragma region 顶点数据
	// 顶点数据 后三项为垂直于顶点表面的法向量（由于顶点本身没有表面，因此一般要利用周围顶点组成表面进行叉乘计算，此处为简单立方体，直接手工添加） 
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

#pragma region 缓存对象
	// 添加作用域，使跳出作用域后局部变量调用析构函数（主要是适应封装的VertexArray和VertexBuffer类，否则可能导致窗口关闭时仍未释放变量）
	{
		// 初始化缓存对象
		//unsigned int VBO;
		//glGenBuffers(1, &VBO);
		//unsigned int VAO;
		//glGenVertexArrays(1, &VAO);
		//// 1. 绑定顶点数组对象
		//glBindVertexArray(VAO);
		// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		VertexBuffer vbo(vertices, sizeof(vertices));
		// 3. 设定顶点属性指针
		// 位置属性
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		//// 法向量属性
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		//// 纹理属性
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);
		VertexArrayLayout layout;
		layout.Push<float>({ 3, 3, 2 });
		VertexArray vao;
		vao.AddBuffer(vbo, layout);

		// 光源VAO、VBO
		unsigned int lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定已经设置好顶点数据的VBO
		vbo.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 忽略顶点数据中的法向量数据
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
		); // 光照各分量对cube的作用

		DirectionalLight dirLight(glm::vec3(0, -1.0f, 0),
			glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

		SpotLight spotLight(glm::vec3(0.0, 3.0f, 0.0), glm::vec3(0, -1.0f, 0), glm::cos(glm::radians(10.0f)),
			glm::cos(glm::radians(30.0f)), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

		// 纹理
		unsigned int diffuseMap = TextureUtil::LoadTexture("./Image/box_container.png", 0);
		Material cubeMaterial(
			&cubeShader,
			glm::vec3(1.0f, 0.5f, 0.31f),
			0, // 漫反射贴图纹理单元索引（此处为第0个纹理单元）
			/*glm::vec3(0.5f, 0.5f, 0.5f),*/
			1, // 镜面光照贴图纹理单元索引（此处为第1个纹理单元）
			2, // 放射光贴图
			32.0f
		);
		unsigned int specularMap = TextureUtil::LoadTexture("./Image/box_container_specular.png", 1);
		unsigned int emmisionMap = TextureUtil::LoadTexture("./Image/matrix.jpg", 2);

	#pragma endregion VAO，VBO，EBO

	#pragma region 渲染

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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// UI帧
			UInewFrame();
			// UI界面
			{
				ImGui::Begin("Test Attenuation");
				ImGui::InputFloat("linear", &cubeLight.linear);
				ImGui::InputFloat("quadratic", &cubeLight.quadratic);
				ImGui::End();
			}

			// 激活着色器
			cubeShader.use();
			// 设置uniform
			cubeShader.setFloat3("objectColor", 1.0f, 0.5f, 0.31f);
			cubeShader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
			cubeShader.setVec3("light.Position", lightPos); // lightPos
			cubeShader.setVec3("viewPos", camera.Position);
			// 光照部分
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
			// 绑定纹理
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			// 创建矩阵
			glm::mat4 model = glm::mat4(1.0f);
			cubeShader.setMat4("model", model);
			glm::mat4 view = camera.GetViewMatrix();
			cubeShader.setMat4("view", view);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			cubeShader.setMat4("projection", projection);
			// 渲染箱子
			//glBindVertexArray(VAO);
			vao.Bind();
			// 循环绘制多个立方体
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
			// 绘制地面
			// TODO

			// 激活光源着色器
			lightShader.use();
			// 设置光源矩阵
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			// 渲染光源
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// 渲染UI
			UIrender();
			// 检查并调用事件，交换缓冲
			glfwSwapBuffers(window);

			// 检查触发什么事件，更新窗口状态
			glfwPollEvents();
		}

		// 清除UI
		UIclear();

		//glDeleteVertexArrays(1, &VAO);
		glDeleteVertexArrays(1, &lightVAO);
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
