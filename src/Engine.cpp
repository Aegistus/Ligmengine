#include <Engine.h>
#include <thread>
#include <chrono>
#include <Types.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Ligmengine
{
	void Engine::Startup()
	{
		graphics.Startup();
		input.Startup();
	}

	void Engine::Shutdown()
	{
		input.Shutdown();
		graphics.Shutdown();
	}

	void Engine::RunGameLoop()
	{
		while (true)
		{
			input.Update();
			// UpdateCallback()
			graphics.Draw();

			// Manage timestep
			double timer = glfwGetTime();
		}
	}
}

