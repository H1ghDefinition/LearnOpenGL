#include <glad/glad.h>
#include <GLFW/glfw3.h> // 在包含GLFW前需要包含GLAD
#include <iostream>

// 顶点着色器代码 为了OpenGL能够使用，需要运行时动态编译，因此存储在一个C字符串中
const char* vertexShaderSource = "#version 330 core\n" // 着色器起始于一个版本声明，这里声明使用3.30版本并使用核心模式
"layout (location = 0) in vec3 aPos;" // 声明输入顶点属性（vec3输入变量aPos）
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// 片段着色器代码
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// 初始化GLFW
	glfwInit();
	// 主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// 次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 使用核心模式Core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//for Mac

	// glfwCreateWindow返回一个GLFWwindow类的实例
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to create GLFW window" << std::endl;
		// 释放/删除分配的所有资源
		glfwTerminate();
		return -1;
	}

	// 将窗口上下文(Context)设置为当前线程的主上下文
	glfwMakeContextCurrent(window);

	// 一般在创建窗口后、渲染循环初始化前注册回调函数
	// 注册窗口大小调整时的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 初始化GLAD 为了用GLAD管理OpenGL的函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 创建并编译顶点着色器
	unsigned int vertexShader; // 顶点着色器ID
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // 指定着色器类型
	glShaderSource(
		vertexShader, // 要编译的着色器对象
		1, // 传递的源码字符串数量
		&vertexShaderSource, // 顶点着色器源码
		NULL
	);
	glCompileShader(vertexShader);

	// 检测着色器编译时错误输出
	int success; // 记录是否成功编译
	char infoLog[512]; // 储存错误消息
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 检查是否编译成功
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // 获取错误信息
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "vertexShader compile SUCCESS" << std::endl;
	}

	//片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//检查片段着色器是否编译错误
	glGetShaderiv(fragmentShader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else {
		std::cout << "fragmentShader complie SUCCESS" << std::endl;
	}

	// 创建着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// 链接
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检测链接着色器程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKED_FAILED\n" << infoLog << std::endl;
	}
	else {
		std::cout << "shaderProgram complie SUCCESS" << std::endl;
	}
	// 把着色器对象链接到程序对象后，删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 绘制三角形相关准备
	// 定义一个float数组作为顶点数据
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	// 生成顶点缓冲对象VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO); // glGenBuffers(缓冲区绑定对象目标数量，缓冲区对象名称(ID))
	// 生成顶点数组对象VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);	
	// 绑定VAO
	glBindVertexArray(VAO);
	// 将新创建的缓冲绑定到GL_ARRAY_BUFFER目标上（指定缓冲类型）
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 将顶点数据保存到VBO上
	glBufferData(
		GL_ARRAY_BUFFER, // 目标缓冲类型
		sizeof(vertices), // 传输数据的大小
		vertices, // 发送的实际数据
		GL_STATIC_DRAW // 管理给定的数据的方式
	);
	// 告诉OpenGL如何把顶点数据链接到顶点着色器的顶点属性上
	glVertexAttribPointer(
		0, // 指定要配置的Location
		3, // 指定顶点属性的大小
		GL_FLOAT, // 指定数据的类型
		GL_FALSE, // 是否希望数据被标准化
		3 * sizeof(float), // 连续的顶点属性组之间的间隔
		(void*)0 // 位置数据在缓冲中起始位置的偏移量
	);
	glEnableVertexAttribArray(0);


	// 渲染循环
	while (!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
	{
		// 检查输入
		processInput(window);

		// 渲染指令
		// 这里清空颜色缓冲（使用自定义的颜色清空屏幕）
		glClearColor(238 / (255.0f), 232 / (255.0f), 170 / (255.0f), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// 绘制三角形
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // 对VBO内部数据进行解释
		glDrawArrays(
			GL_TRIANGLES, // 图元类型
			0, // 顶点数组的起始索引
			3 // 绘制多少个顶点
		);

		// 交换颜色缓冲，在此用来绘制屏幕
		glfwSwapBuffers(window);

		// 检查是否触发事件、更新窗口状态，并调用对应的回调函数
		glfwPollEvents();
	}

	// 渲染循环结束后释放/删除分配的所有资源
	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteFramebuffers(1, &VBO);
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
}