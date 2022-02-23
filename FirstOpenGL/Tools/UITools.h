#pragma once
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void UIinit(GLFWwindow* window)
{
	// Create a Dear ImGui context, setup some options
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Initialize Platform + Renderer backends (here: using imgui_impl_win32.cpp + imgui_impl_dx11.cpp)
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UInewFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UIrender()
{
	// Rendering GUI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIclear()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}