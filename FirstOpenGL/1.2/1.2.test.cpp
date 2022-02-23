#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "UITools.h"

bool GammaCorrection = false;

const char* vertexShaderSource = "#version 330 core\n" 
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.xyz, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"uniform bool Gamma;\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   if(Gamma)\n"
"   {\n"
"		FragColor = vec4(pow(ourColor.rgb, vec3(1.0/2.2)), 1.0);\n"
"	}\n"
"	else\n"
"		FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Rectangle", NULL, NULL);
	if (window == NULL)
	{
		// 生成错误则输出错误信息
		std::cout << "Failed to create GLFW window" << std::endl;
		// 释放/删除分配的所有资源
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//// Create a Dear ImGui context, setup some options
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();

	//// Initialize Platform + Renderer backends (here: using imgui_impl_win32.cpp + imgui_impl_dx11.cpp)
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init("#version 330");

	UIinit(window);

	float vertices[] = {
		// 位置              
		 0.5f, -0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f, 
		 0.0f,  0.5f, 0.0f,  
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//unsigned int indices[] = {
	//	0, 1, 3, // 第一个三角形
	//	1, 2, 3 // 第二个三角形
	//};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBOs[2];
	glGenBuffers(2, VBOs);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int length = 512;
	char infoLog[512];
	int success = 0;

	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILE ERROR :" << infoLog << std::endl;
	}

	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILE ERROR :" << infoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM LINK ERROR :" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "Gamma"), GammaCorrection);

	GLint max_combined_texture_image_units;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units); // 192
	GLint max_vertex_texture_image_units;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_vertex_texture_image_units); // 32
	std::cout << max_combined_texture_image_units << " " << max_vertex_texture_image_units << std::endl;
	GLint texture0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture0); // 32
	std::cout << texture0;

	// Gamma矫正
	ImVec4 v_colors[3];
	v_colors[0] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	v_colors[1] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	v_colors[2] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);

		//// Start the Dear ImGui frame
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();
		UInewFrame();

		{
			ImGui::Begin("Hello, world!");

			ImGui::ColorEdit3("Vertex_0", (float*)&v_colors[0]);
			ImGui::ColorEdit3("Vertex_1", (float*)&v_colors[1]);
			ImGui::ColorEdit3("Vertex_2", (float*)&v_colors[2]);

			ImGui::Checkbox("Open gamma correction or not?", &GammaCorrection);

			ImGui::End();
		}


		// Do Rendering
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);


		// 修改color值
		for (int i = 0; i < 3; ++i)
		{
			colors[0 + 3 * i] = v_colors[i].x * v_colors[i].w;
			colors[1 + 3 * i] = v_colors[i].y * v_colors[i].w;
			colors[2 + 3 * i] = v_colors[i].z * v_colors[i].w;
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STREAM_DRAW);

		glUniform1i(glGetUniformLocation(shaderProgram, "Gamma"), GammaCorrection);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//// Rendering GUI
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		UIrender();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	UIclear();

	glDeleteProgram(shaderProgram);
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
}