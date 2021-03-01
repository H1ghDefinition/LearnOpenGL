#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

// 添加时间差deltaTime变量 用于平衡因配置问题导致的不同机器移动速度不同问题
// 如果deltaTime很大，说明上一帧渲染花费更多时间，所以当前帧的速度需要变得更高来平衡渲染花费的时间
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

const float screenWidth = 800;
const float screenHeight = 600;

// 摄像机参数
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 方向向量
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 摄像机鼠标移动参数
bool firstMouse = true; // 是否第一次获取鼠标输入
float yaw = -90.0f; // 偏航角，由于默认摄像机指向Z轴正向（方向向量cameraFront Z轴负向），因此此处由X轴正向顺时针旋转90°（顺时针负数 逆时针正数）
float pitch = 0.0f; // 俯仰角
float lastX = 400, lastY = 300; // 上一帧鼠标位置参数 默认位于屏幕中心

// 摄像机鼠标滚轮缩放参数
float fov = 45.0f;
float minfov = 1.0f;
float maxfov = 60.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int main()
{
	// 初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "1.6", NULL, NULL);
	if (window == NULL)
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to create GLFW window" << std::endl;
		// 释放/删除分配的所有资源
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 配置隐藏光标并捕捉
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 着色器
	Shader myShader("./1.6/vert162.txt", "./1.6/frag162.txt");

	// 顶点数据
	//float vertices[] = {
	//	//     ---- 位置 ----   - 纹理坐标 -
	//		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
	//		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // 右下
	//		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // 左下
	//		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // 左上
	//};
	//unsigned int indices[] = {
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	}; // 一个立方体 6个面*每个面2个三角形*每个三角形3个顶点 共36个顶点

	// 立方体位置
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

	// 顶点输入
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// 顶点属性多了纹理坐标，需要添加解释
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture1
	// 生成、绑定纹理
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	// 环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 根据图片加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./1.4/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// 生成纹理和相应多级渐远纹理后，释放图像的内存
	stbi_image_free(data);

	// texture2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true); // 加载图像前设置 进行Y轴翻转
	/*data = stbi_load("./1.4/lol.png", &width, &height, &nrChannels, 0);*/
	data = stbi_load("./Image/Elaina.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false); // 加载后关闭Y轴翻转

	// 用着色器类 找到对应名字的uniform变量 
	myShader.use(); // 在设置uniform前要先激活着色器
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1); // 设置uniform为1，表明该着色器采样器属于纹理单元1，渲染循环中使用纹理单元GL_TEXTURE1进行绑定

	// 摄像机设置

	//// 摄像机位置
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//// 摄像机的方向向量（实际指向从摄像机到目标向量的相反方向）
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	//// 摄像机的右向量（代表摄像机空间的x轴正方向）：通过一个指向y轴正向的up向量叉乘方向向量 决定右向量方向
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//// 摄像机的上向量
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// glm已经提供创建lookAt矩阵的API，我们只需提供一个摄像机位置、一个目标位置和一个表示世界空间中的上向量的向量
	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//myShader.setMat4("view", view);

	glEnable(GL_DEPTH_TEST); // 启用深度测试
	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 计算deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(238 / (255.0f), 232 / (255.0f), 170 / (255.0f), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 位或 进行深度缓冲清除

		// 在glDraw之前绑定纹理即可，其会自动将纹理赋值给片段着色器中的uniform类型采样器
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// 激活着色器
		myShader.use();

		// 创建MVP矩阵
		//glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		/*projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);*/
		projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);
		
		// 使用lookAt矩阵定义摄像机位置 移动摄像机
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(
			cameraPos,
			cameraPos + cameraFront, // 保证摄像机注视着目标方向
			cameraUp
		);

		// 使用着色器类赋值uniform
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		// 绘制
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		// 循环绘制多个立方体
		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * angle), glm::vec3(1.0f, 0.3f, 0.5f));
			myShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 交换缓冲
		glfwSwapBuffers(window);

		// 
		glfwPollEvents();
	}

	// 释放/删除分配的所有资源
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 是否按下返回键
		glfwSetWindowShouldClose(window, true);

	// 摄像机控制
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// 0.检查是否第一次获取鼠标输入
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	} // 如果缺少这一步，在鼠标第一次移动进窗口时，鼠标回调函数调用，会产生一个很大的偏移量，指向大幅度跳动到移动进窗口的鼠标位置

	// 1.计算鼠标距离上一帧的偏移量
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，鼠标在窗口中上移的时候，ypos 变量值是逐渐减小的。
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// 2.把偏移量添加到摄像机的俯仰角和偏航角
	yaw += xoffset;
	pitch += yoffset;

	// 3.添加限制值，不让用户看到高于89度的地方
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// 4.通过俯仰角和偏航角来计算得到真正的方向向量
	glm::vec3 front; // 单位向量
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= minfov && fov <= maxfov)
		fov -= yoffset;
	if (fov <= minfov)
		fov = minfov;
	if (fov >= maxfov)
		fov = maxfov;
}