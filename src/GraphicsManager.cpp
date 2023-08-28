#include <iostream>
#include <GraphicsManager.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace Ligmengine
{


	void GraphicsManager::Startup()
	{
		glfwInit();
		// We don't want GLFW to set up a graphics API.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// Create the window.
		window = glfwCreateWindow(window_width, window_height, window_name, window_fullscreen ? glfwGetPrimaryMonitor() : 0, 0);
		glfwSetWindowAspectRatio(window, window_width, window_height);
		if (!window)
		{
			spdlog::error("Failed to create a window.");
			glfwTerminate();
		}
	}

	void GraphicsManager::Shutdown()
	{
		glfwTerminate();
	}

	void GraphicsManager::Draw()
	{

	}
}
