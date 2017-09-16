#include "Window.h"


Window::Window(string name, int width, int height) :
screenWidth(width),
screenHeight(height)
{
	// Init GLFW
	glfwInit();
	glfwSetTime(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	m_pWindow = glfwCreateWindow(screenWidth, screenHeight, name.c_str(), nullptr, nullptr); // Windowed
	if (m_pWindow == nullptr) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_pWindow);	

#if defined(NANOGUI_GLAD)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Could not initialize GLAD!");
	glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
#endif
}

Window::~Window()
{
	
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_pWindow);
}

void Window::SetKeyCallback(GLFWkeyfun keyFunc)
{
	glfwSetKeyCallback(m_pWindow, keyFunc);
}

void Window::SetCursorPosCallback(GLFWcursorposfun mouseFunc)
{
	glfwSetCursorPosCallback(m_pWindow, mouseFunc); 
}

void Window::SetScrollCallback(GLFWscrollfun scroolFunc)
{
	glfwSetScrollCallback(m_pWindow, scroolFunc);
}

void Window::SetWindowSizeCallback(GLFWwindowsizefun windowSizeFun)
{
	glfwSetWindowSizeCallback(m_pWindow, windowSizeFun);
}

void Window::SetMouseButtonCallback(GLFWmousebuttonfun cbfun)
{
	glfwSetMouseButtonCallback(m_pWindow, cbfun);
}

void Window::SetCharCallback(GLFWcharfun cbfun)
{
	glfwSetCharCallback(m_pWindow, cbfun);
}

void Window::SetDropCallback(GLFWdropfun cbfun)
{
	glfwSetDropCallback(m_pWindow, cbfun);
}

void Window::SetFrameBufferSizeCallback(GLFWframebuffersizefun cbfun)
{
	glfwSetFramebufferSizeCallback(m_pWindow, cbfun);
}

GLuint Window::GetWidth()
{
	return screenWidth;
}

GLuint Window::GetHeight()
{
	return screenHeight;
}

GLFWwindow* Window::GetGLFWWindow()
{
	return m_pWindow;
}

void Window::SetWidthAndHeight(GLuint width, GLuint height)
{
	screenWidth = width;
	screenHeight = height;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

extern bool isRightButtonDown;
extern bool isLeftButtonDown;
extern GLfloat lastX;
extern GLfloat lastY;

/*void Window::DoUI()
{
	int x = screenWidth - 200;
	int y = screenHeight - 23;

	// imgui
	unsigned char button = 0;
	if (isLeftButtonDown)
		button = IMGUI_MBUT_LEFT;
	else if (isRightButtonDown)
		button = IMGUI_MBUT_RIGHT;

	imguiBeginFrame(lastX, screenHeight - lastY, button, 0);

	x += 180;
	int fontHeight = 13;

	//暂定：输出模型信息等

	x -= 180;
	int uiOffset = 250;
	int uiBorder = 20;
	int uiWidth = 200;
	int uiHeight = screenHeight - uiOffset - uiBorder * 3;
	int uiLeft = uiBorder;

	static int scroll = 0;

	imguiBeginScrollArea("Options", uiLeft, screenHeight - uiBorder - uiHeight - uiOffset - uiBorder, uiWidth, uiHeight, &scroll);
	imguiSeparatorLine();

	bool flag = false;
	if (imguiCheck("Wireframe", flag))
		flag = !flag;

	imguiEndFrame();
//	imguiGraphDraw(screenWidth, screenHeight);
}*/