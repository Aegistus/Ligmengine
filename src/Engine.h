#pragma once
#include <functional>
#include <GraphicsManager.h>
#include <InputManager.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Ligmengine
{
	static const float TIME_STEP = ((float) 1 / 60);
	typedef std::function<void()> UpdateCallback;

	class Engine
	{
		public:
			bool quit = false;
			GraphicsManager graphics;
			InputManager input;

			void Startup(const UpdateCallback& callback);

			void Shutdown();

			void RunGameLoop(const UpdateCallback& callback);

	};
	
	inline Engine gEngine;

}
