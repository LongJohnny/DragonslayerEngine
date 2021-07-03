#ifndef GUI_H
#define GUI_H

#include "../view/Camera.h"
#include "HierarchyPanel.h"
#include "ObjectPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

class GUI {

private: 
	ImGuiIO& imGuiIO;
	HierarchyPanel* hierarchyPanel = nullptr;
	ObjectPanel* objectPanel = nullptr;
	
	void setDefaultTheme();
	void setDefaultStyle();
	void loadFonts();

public:
	GUI(ImGuiIO& imGuiIO, GLFWwindow* window);
	~GUI();

	void drawUI(GLuint id, Camera& camera);
};

#endif
