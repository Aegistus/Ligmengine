#include <Engine.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <Types.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace Ligmengine
{
	void Engine::Startup()
	{
		graphics.Startup();
		input.Startup();
		RunGameLoop();
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
			double frameStart = glfwGetTime();
			spdlog::info(frameStart);
			input.Update();
			// UpdateCallback()
			graphics.Draw();

			// Manage timestep
			double sleepTime = TIME_STEP - (glfwGetTime() - frameStart);
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
		}
		Shutdown();
	}
}

