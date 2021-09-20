#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "core/Hierarchy.h"
#include "gui/GUI.h"
#include "meshes/SkyBox.h"
#include "view/EditorCamera.h"

using namespace core;

class Engine {

private:
	// Only derived classed have access to this members
	GLFWwindow* window = nullptr;
	Hierarchy* hierarchy;
	EditorCamera* editorCamera = nullptr;
	GUI* gui = nullptr;
	SkyBox* skybox = nullptr;
	int windowWidth = 0;
	int windowHeight = 0;
	double elapsedTime = 0.0;
	std::function<void(std::function<void()> renderScene)> preRender;

	/* Sets up everything related to glfw */
	void setupGLFW();

	/* Sets up everything related to glew */
	void setupGLEW();

	/* Sets up everything related to glfw */
	void setupOpenGL();

	/* Sets up the editorCamera and sceneGraph */
	void setupScene();

	/* Sets up everything related to ImGUI*/
	void setupGUI();

	/* Frees any memory allocated on the heap */
	void freeResources();

	/* Called before main loop
	*/
	virtual void start() {/*EMPTY*/ }

	/* Called every frame
	*/
	virtual void update() {/*EMPTY*/ }

	/* Called after main loop
	* Should be used to free any memory allocated in start or update
	*/
	virtual void end() {/*EMPTY*/ }

protected:
	GLFWwindow* getWindow(); 
	EditorCamera* getCamera();
	int getWindowWidth();
	int getWindowHeight();
	double getElapsedTime();
	GUI* getGui();
	void setSkyBox(const std::vector<std::string>& facesFilePath);

public:
	/* If called, the lambda preRender will be called before the rendering of the scene, useful for shadow mapping for example*/
	void setPreRender(std::function<void(std::function<void()> renderScene)> preRender);

	void updateWindow(float width, float height);

	void run();
};


#endif
