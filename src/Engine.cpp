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
	void Engine::Startup(const UpdateCallback& startupCallback, const UpdateCallback& callback)
	{
		graphics.Startup();
		input.Startup();
		soundManager.Startup();
		startupCallback();
		RunGameLoop(callback);
	}

	void Engine::Shutdown()
	{
		input.Shutdown();
		graphics.Shutdown();
		soundManager.Shutdown();
	}

	void Engine::RunGameLoop(const UpdateCallback& callback)
	{
		while (!quit)
		{
			if (glfwWindowShouldClose(graphics.window))
			{
				quit = true;
			}
			double frameStart = glfwGetTime();
			spdlog::info(frameStart);
			input.Update();
			// UpdateCallback
			graphics.Draw();
			callback();

			// late updates
			input.LateUpdate();

			// Manage timestep
			double sleepTime = TIME_STEP - (glfwGetTime() - frameStart);
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
		}
		Shutdown();
	}
}

