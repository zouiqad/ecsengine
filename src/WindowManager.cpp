#include "WindowManager.h"

#include <bitset>
#include <iostream>

#include "core/Mediator.h"


extern Mediator gMediator;

void windowResizeCallBack(GLFWwindow* window, int windowWidth, int windowHeight)
{
	glViewport(0, 0, windowWidth, windowHeight);
}

void windowMouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	WindowManager *windowManager = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
	if (windowManager->mFirstMouse)
	{
		windowManager->mLastMouseX = xpos;
		windowManager->mLastMouseY = ypos;
		windowManager->mFirstMouse = false;
	}

	double xoffset = xpos - windowManager->mLastMouseX;
	double yoffset = windowManager->mLastMouseY - ypos;

	if(xoffset != 0 || yoffset != 0) {
		Event event(Events::Window::MOUSEMOVE);
		event.SetParam(Events::Window::Input::MOUSE_XOFFSET, xoffset);
		event.SetParam(Events::Window::Input::MOUSE_YOFFSET, yoffset);
		gMediator.SendEvent(event);
	}

	windowManager->mLastMouseX = xpos;
	windowManager->mLastMouseY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
}

// TODO: Return error to caller
bool WindowManager::Init(
	std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight, unsigned int windowPositionX,
	unsigned int windowPositionY)
{
	glfwInit();

	mWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

	if (!mWindow) {
		std::cerr << "Failed to create GLFW window.\n";
		glfwTerminate ();
		return false;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create OpenGL Context
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD.\n";
		return false;
	}

	// Set callbacks
	glfwSetWindowSizeCallback(mWindow, windowResizeCallBack);
	glfwSetCursorPosCallback(mWindow, windowMouseCallBack);

	// Configure OpenGL
	glViewport (0, 0, windowWidth, windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// Disable cursor
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mLastMouseX = windowWidth / 2.0;
	mLastMouseY = windowHeight / 2.0;


	glfwSetWindowUserPointer(mWindow, this);
	return true;
}


void WindowManager::Update()
{
	glfwSwapBuffers(mWindow);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void WindowManager::Shutdown()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void WindowManager::ProcessEvents()
{
	glfwPollEvents();

	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		gMediator.SendEvent(Events::Window::QUIT);
	}

	std::bitset<8> currentButtons;
	currentButtons.set(static_cast<std::size_t>(InputButtons::W),
						glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS);
	currentButtons.set(static_cast<std::size_t>(InputButtons::A),
						glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS);
	currentButtons.set(static_cast<std::size_t>(InputButtons::S),
						glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS);
	currentButtons.set(static_cast<std::size_t>(InputButtons::D),
						glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS);
	currentButtons.set(static_cast<std::size_t>(InputButtons::Q),
						glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS);
	currentButtons.set(static_cast<std::size_t>(InputButtons::E),
						glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS);


	if (currentButtons != mButtons)
	{
		mButtons = currentButtons;
		Event event(Events::Window::KEYDOWN);
		event.SetParam(Events::Window::Input::KEYS_DOWN, mButtons);
		gMediator.SendEvent(event);
	}
}


