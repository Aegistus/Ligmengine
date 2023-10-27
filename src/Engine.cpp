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
		scriptManager.Startup();
		physics.Startup();
		startupCallback();
		scriptManager.LoadScript("LoadAssets", "scripts/LoadAssets.lua");
		scriptManager.RunScript("LoadAssets");
		scriptManager.LoadScript("Start", "scripts/Start.lua");
		scriptManager.RunScript("Start");
		RunGameLoop(callback);
	}

	void Engine::Shutdown()
	{
		input.Shutdown();
		graphics.Shutdown();
		soundManager.Shutdown();
		physics.Shutdown();
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
			//spdlog::info(frameStart);
			input.Update();
			scriptManager.Update();
			graphics.Draw();
			physics.Update();
			callback();
			// late updates
			input.LateUpdate();

			// Manage timestep
			double sleepTime = TIME_STEP - (glfwGetTime() - frameStart);
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
			deltaTime = glfwGetTime() - frameStart;
		}
		Shutdown();
	}
}
