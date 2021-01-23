#include <glad/glad.h>
#include <GLFW/glfw3.h> // �ڰ���GLFWǰ��Ҫ����GLAD
#include <iostream>

// ������ɫ������ Ϊ��OpenGL�ܹ�ʹ�ã���Ҫ����ʱ��̬���룬��˴洢��һ��C�ַ�����
const char* vertexShaderSource = "#version 330 core\n" // ��ɫ����ʼ��һ���汾��������������ʹ��3.30�汾��ʹ�ú���ģʽ
"layout (location = 0) in vec3 aPos;" // �������붥�����ԣ�vec3�������aPos��
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Ƭ����ɫ������
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
	// ��ʼ��GLFW
	glfwInit();
	// ���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// �ΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// ʹ�ú���ģʽCore-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//for Mac

	// glfwCreateWindow����һ��GLFWwindow���ʵ��
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		// ���ɴ��������������Ϣ
		std::cout << "Failed to create GLFW window" << std::endl;
		// �ͷ�/ɾ�������������Դ
		glfwTerminate();
		return -1;
	}

	// ������������(Context)����Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);

	// һ���ڴ������ں���Ⱦѭ����ʼ��ǰע��ص�����
	// ע�ᴰ�ڴ�С����ʱ�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ��ʼ��GLAD Ϊ����GLAD����OpenGL�ĺ���ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ���������붥����ɫ��
	unsigned int vertexShader; // ������ɫ��ID
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // ָ����ɫ������
	glShaderSource(
		vertexShader, // Ҫ�������ɫ������
		1, // ���ݵ�Դ���ַ�������
		&vertexShaderSource, // ������ɫ��Դ��
		NULL
	);
	glCompileShader(vertexShader);

	// �����ɫ������ʱ�������
	int success; // ��¼�Ƿ�ɹ�����
	char infoLog[512]; // ���������Ϣ
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // ����Ƿ����ɹ�
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // ��ȡ������Ϣ
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "vertexShader compile SUCCESS" << std::endl;
	}

	//Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//���Ƭ����ɫ���Ƿ�������
	glGetShaderiv(fragmentShader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else {
		std::cout << "fragmentShader complie SUCCESS" << std::endl;
	}

	// ������ɫ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// ����
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// ���������ɫ�������Ƿ�ʧ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKED_FAILED\n" << infoLog << std::endl;
	}
	else {
		std::cout << "shaderProgram complie SUCCESS" << std::endl;
	}
	// ����ɫ���������ӵ���������ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// �������������׼��
	// ����һ��float������Ϊ��������
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	// ���ɶ��㻺�����VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO); // glGenBuffers(�������󶨶���Ŀ����������������������(ID))
	// ���ɶ����������VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);	
	// ��VAO
	glBindVertexArray(VAO);
	// ���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�ָ���������ͣ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���������ݱ��浽VBO��
	glBufferData(
		GL_ARRAY_BUFFER, // Ŀ�껺������
		sizeof(vertices), // �������ݵĴ�С
		vertices, // ���͵�ʵ������
		GL_STATIC_DRAW // ������������ݵķ�ʽ
	);
	// ����OpenGL��ΰѶ����������ӵ�������ɫ���Ķ���������
	glVertexAttribPointer(
		0, // ָ��Ҫ���õ�Location
		3, // ָ���������ԵĴ�С
		GL_FLOAT, // ָ�����ݵ�����
		GL_FALSE, // �Ƿ�ϣ�����ݱ���׼��
		3 * sizeof(float), // �����Ķ���������֮��ļ��
		(void*)0 // λ�������ڻ�������ʼλ�õ�ƫ����
	);
	glEnableVertexAttribArray(0);


	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) // ���GLFW�Ƿ�Ҫ���˳�
	{
		// �������
		processInput(window);

		// ��Ⱦָ��
		// ���������ɫ���壨ʹ���Զ������ɫ�����Ļ��
		glClearColor(238 / (255.0f), 232 / (255.0f), 170 / (255.0f), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// ����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // ��VBO�ڲ����ݽ��н���
		glDrawArrays(
			GL_TRIANGLES, // ͼԪ����
			0, // �����������ʼ����
			3 // ���ƶ��ٸ�����
		);

		// ������ɫ���壬�ڴ�����������Ļ
		glfwSwapBuffers(window);

		// ����Ƿ񴥷��¼������´���״̬�������ö�Ӧ�Ļص�����
		glfwPollEvents();
	}

	// ��Ⱦѭ���������ͷ�/ɾ�������������Դ
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // �Ƿ��·��ؼ�
		glfwSetWindowShouldClose(window, true);
}