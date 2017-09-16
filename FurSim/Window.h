#pragma once
// GL
//#include <GLEW\glew.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
//STL
#include <string>
#include <iostream>

//#include "UI/imgui.h"

using namespace std;

class Window
{
public:
	Window(string name, int width, int height);
	~Window();
	bool ShouldClose();
	void SetKeyCallback(GLFWkeyfun keyFunc);
	void SetCursorPosCallback(GLFWcursorposfun mouseFunc);
	void SetScrollCallback(GLFWscrollfun scroolFunc); 
	void SetWindowSizeCallback(GLFWwindowsizefun windowSizeFun);
	void SetMouseButtonCallback(GLFWmousebuttonfun cbfun);
	void SetCharCallback(GLFWcharfun cbfun);
	void SetDropCallback(GLFWdropfun cbfun);
	void SetFrameBufferSizeCallback(GLFWframebuffersizefun cbfun);
	void DoUI();

	GLuint GetWidth();
	GLuint GetHeight();
	GLFWwindow* GetGLFWWindow();
	void SetWidthAndHeight(GLuint width, GLuint height);

	void SwapBuffers();

private:
	GLuint screenWidth;
	GLuint screenHeight;
	GLFWwindow* m_pWindow;
	
};

