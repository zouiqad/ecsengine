#include "WindowManager.h"

#include <bitset>
#include <iostream>

#include "core/Mediator.h"


extern Mediator gMediator;


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


	// Configure OpenGL
	glViewport (0, 0, windowWidth, windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	return true;
}


void WindowManager::Update()
{
	glfwSwapBuffers(mWindow);
}


void WindowManager::Shutdown()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void WindowManager::ProcessEvents()
{
	glfwPollEvents();

	bool buttonStateChanged = true;

	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
	{
		gMediator.SendEvent(Events::Window::QUIT);
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_W))
	{
		mButtons.set(static_cast<std::size_t>(InputButtons::W));
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_A))
	{
		mButtons.set(static_cast<std::size_t>(InputButtons::A));
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_S))
	{
		mButtons.set(static_cast<std::size_t>(InputButtons::S));
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_D))
	{
		mButtons.set(static_cast<std::size_t>(InputButtons::D));
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Q))
	{
		mButtons.set(static_cast<std::size_t>(InputButtons::Q));
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_E))
	{
		mButtons.set(static_cast<std::size_t>(InputButtons::E));
	}
	else
	{
		buttonStateChanged = false;
	}

	if (buttonStateChanged)
	{
		Event event(Events::Window::INPUT);
		event.SetParam(Events::Window::Input::INPUT, mButtons);
		gMediator.SendEvent(event);
	}
}
