#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <memory>

#include "Camera.h"
#include "Material.h"
#include "UITools.h"
#include "Model.h"
#include "Light.h"
#include "glUtil.h"
#include "SkyBox.h"
#include "CubemapFrameBuffer.h"
using namespace glUtil::DebugUtil;

// path
const std::string RESOURCES_PATH = "./DynamicCubemap/resources/";

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
std::shared_ptr<Camera> camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

// ���ھ۽�״̬
bool isMouseFocus = true;

// ��������
std::shared_ptr<Shader> normalSphereShader;
std::shared_ptr<Shader> reflectSphereShader;
std::shared_ptr<Shader> refractSphereShader;
std::shared_ptr<Shader> skyboxShader;
std::shared_ptr<Model> sphere;
std::shared_ptr<Model> cube;
std::shared_ptr<ColorMaterial> blinnPhongMaterial;
std::shared_ptr<PointLight> light;
std::shared_ptr<SkyBox> skybox;
std::shared_ptr<CubemapFrameBuffer> cubemapFrameBuffer;
float ior = 1.015;

bool fresnelAccurate = false;
bool refractEffect = false;
float radius1 = 2.0f;
float radius2 = 6.0f;
float speed1 = 3.0f;
float speed2 = 6.0f;
float scale1 = 0.7f;
float scale2 = 0.5f;

void UIcontent()
{
	ImGui::Begin("OpenGL Demo UI");
	ImGui::DragFloat("ior", &ior, 0.002, 1.00, 10.00);
	ImGui::Checkbox("refract effect active", &refractEffect);
	ImGui::Checkbox("fresnel accurate", &fresnelAccurate);
	ImGui::DragFloat("sphere 1 radius", &radius1, 0.02, 1.0, 20.0);
	ImGui::DragFloat("speed 1", &speed1, 0.02, 1.0, 20.0);
	ImGui::DragFloat("scale 1", &scale1, 0.02, 0.1, 2.0);
	ImGui::DragFloat("sphere 2 radius", &radius2, 0.02, 1.0, 20.0);
	ImGui::DragFloat("speed 2", &speed2, 0.02, 1.0, 20.0);
	ImGui::DragFloat("scale 2", &scale2, 0.02, 0.1, 2.0);
	ImGui::End();
}

// ������ʼ�� + ���ݴ���
void init()
{
	camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	normalSphereShader = std::make_shared<Shader>((RESOURCES_PATH + "normal.vert").c_str(), (RESOURCES_PATH + "normal.frag").c_str());
	reflectSphereShader = std::make_shared<Shader>((RESOURCES_PATH + "reflect.vert").c_str(), (RESOURCES_PATH + "reflect.frag").c_str());
	refractSphereShader = std::make_shared<Shader>((RESOURCES_PATH + "refract.vert").c_str(), (RESOURCES_PATH + "refract.frag").c_str());
	skyboxShader = std::make_shared<Shader>((RESOURCES_PATH + "skybox.vert").c_str(), (RESOURCES_PATH + "skybox.frag").c_str());
	sphere = std::make_shared<Model>(RESOURCES_PATH + "sphere.obj");
	cube = std::make_shared<Model>(RESOURCES_PATH + "cube.obj");
	blinnPhongMaterial = std::make_shared<ColorMaterial>(normalSphereShader.get(),
		glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.8, 0.7, 0.7), glm::vec3(1.0, 0.9, 0.8), 64);
	light = std::make_shared<PointLight>(nullptr, glm::vec3(0.0, 5.0, 0.0),
		glm::vec3(0.9, 0.9, 0.9), glm::vec3(0.9, 0.9, 0.9), glm::vec3(0.9, 0.9, 0.9));
	skybox = std::make_shared<SkyBox>(RESOURCES_PATH + "skybox/", 3); // 3������Ԫ
	cubemapFrameBuffer = std::make_shared<CubemapFrameBuffer>(17); // 17������Ԫ

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// ����
	normalSphereShader->use();
	normalSphereShader->setMVP(model, view, projection);
	normalSphereShader->setVec3("material.ambient", blinnPhongMaterial->ambient);
	normalSphereShader->setVec3("material.diffuse", blinnPhongMaterial->diffuse);
	normalSphereShader->setVec3("material.specular", blinnPhongMaterial->specular);
	normalSphereShader->setFloat("material.shininess", blinnPhongMaterial->shininess);
	normalSphereShader->setVec3("light.position", light->position);
	normalSphereShader->setVec3("light.ambient", light->ambient);
	normalSphereShader->setVec3("light.diffuse", light->diffuse);
	normalSphereShader->setVec3("light.specular", light->specular);
	normalSphereShader->setFloat("light.constant", light->constant);
	normalSphereShader->setFloat("light.linear", light->linear);
	normalSphereShader->setFloat("light.quadratic", light->quadratic);
	normalSphereShader->setVec3("viewPos", camera->Position);
	normalSphereShader->disuse();

	reflectSphereShader->use();
	reflectSphereShader->setMVP(model, view, projection);
	reflectSphereShader->setInt("skybox", 17); 
	reflectSphereShader->setVec3("viewPos", camera->Position);
	reflectSphereShader->disuse();

	refractSphereShader->use();
	refractSphereShader->setMVP(model, view, projection);
	refractSphereShader->setInt("skybox", 17); // 0������Ԫ��cubemap
	refractSphereShader->setVec3("viewPos", camera->Position);
	refractSphereShader->disuse();

	skyboxShader->use();
	skyboxShader->setInt("skybox", 3);
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	skyboxShader->disuse();

	// ������ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ��Ⱦ����
void render()
{
	// ֡ʱ���
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	std::shared_ptr<Shader> activeShader = reflectSphereShader;

	if (refractEffect)
	{
		activeShader = refractSphereShader;
	}

	// 1st render: render glass sphere scene
	cubemapFrameBuffer->Bind();
	for (int i = 0; i < 6; ++i)
	{
		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);

		cubemapFrameBuffer->DrawSide(view, projection, i); // clear bit and setup
		// start render
		normalSphereShader->use();
		glm::mat4 rotateModel = glm::rotate(model, currentFrame / radius1 * speed1, glm::vec3(0.0f, 1.0f, 0.0f));
		rotateModel = glm::translate(rotateModel, glm::vec3(radius1, 0.0f, 0.0f));
		rotateModel = glm::scale(rotateModel, glm::vec3(0.7f, 0.7f, 0.7f));
		//normalSphereShader->setMat4("model", rotateModel);
		//normalSphereShader->setMat4("view", view);
		//normalSphereShader->setMat4("projection", projection);
		normalSphereShader->setMVP(rotateModel, view, projection);
		sphere->Draw(*normalSphereShader);
		normalSphereShader->disuse();

		normalSphereShader->use();
		rotateModel = glm::rotate(model, currentFrame / radius2 * speed2, glm::vec3(1.0f, 0.0f, 0.0f));
		rotateModel = glm::translate(rotateModel, glm::vec3(0.0f, radius2, 0.0f));
		rotateModel = glm::scale(rotateModel, glm::vec3(0.5f, 0.5f, 0.5f));
		normalSphereShader->setMVP(rotateModel, view, projection);
		sphere->Draw(*normalSphereShader);
		normalSphereShader->disuse();

		skyboxShader->use();
		view = glm::mat4(glm::mat3(view));
		skyboxShader->setMat4("view", view);
		skyboxShader->setMat4("projection", projection);
		skybox->Bind();
		skybox->Draw(*skyboxShader);
		skyboxShader->disuse();
		skybox->Unbind();
	}

	// 2nd render: render scene again
	cubemapFrameBuffer->Unbind(SCR_WIDTH, SCR_HEIGHT);

	model = glm::mat4(1.0f);
	view = camera->GetViewMatrix();
	projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	normalSphereShader->use();
	glm::mat4 rotateModel = glm::rotate(model, currentFrame / radius1 * speed1, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateModel = glm::translate(rotateModel, glm::vec3(radius1, 0.0f, 0.0f));
	rotateModel = glm::scale(rotateModel, glm::vec3(scale1, scale1, scale1));
	normalSphereShader->setMVP(rotateModel, view, projection);
	sphere->Draw(*normalSphereShader);
	normalSphereShader->disuse();

	normalSphereShader->use();
	rotateModel = glm::rotate(model, currentFrame / radius2 * speed2, glm::vec3(1.0f, 0.0f, 0.0f));
	rotateModel = glm::translate(rotateModel, glm::vec3(0.0f, radius2, 0.0f));
	rotateModel = glm::scale(rotateModel, glm::vec3(scale2, scale2, scale2));
	normalSphereShader->setMVP(rotateModel, view, projection);
	sphere->Draw(*normalSphereShader);
	normalSphereShader->disuse();

	reflectSphereShader->use();
	//reflectSphereShader->setMat4("model", model);
	//reflectSphereShader->setMat4("view", view);
	reflectSphereShader->setMVP(model, view, projection);
	reflectSphereShader->setVec3("viewPos", camera->Position);
	glActiveTexture(GL_TEXTURE0 + 17);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapFrameBuffer->GetTextureID());
	reflectSphereShader->setInt("skybox", 17);
	//sphere->Draw(*reflectSphereShader);
	//cube->Draw(*reflectSphereShader);
	reflectSphereShader->disuse();

	refractSphereShader->use();
	refractSphereShader->setInt("skybox", 17);
	refractSphereShader->setMVP(model, view, projection);
	refractSphereShader->setVec3("viewPos", camera->Position);
	refractSphereShader->setFloat("ior", ior);
	refractSphereShader->setBool("fresnelAccurate", fresnelAccurate);
	//sphere->Draw(*refractSphereShader);
	refractSphereShader->disuse();

	sphere->Draw(*activeShader);

	skyboxShader->use();
	view = glm::mat4(glm::mat3(view));
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	skybox->Bind();
	skybox->Draw(*skyboxShader);
	skybox->Unbind();
	skyboxShader->disuse();
}

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

	// ��ʼ��UI
	UIinit(window);

	// ��ʼ��������Ϣ
	init();

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ��������
		processInput(window);
		// ��Ⱦ����
		render();
		// ��ȾUI
		UInewFrame();
		UIcontent();
		UIrender();
		// ��������
		glfwSwapBuffers(window);
		// ����¼�
		glfwPollEvents();
	}

	UIclear();
	glfwTerminate();
}

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
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(DOWN, deltaTime);
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

		camera->ProcessMouseMovement(xoffset, yoffset);
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
	camera->ProcessMouseScroll(yoffset);
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