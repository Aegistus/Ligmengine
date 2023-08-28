#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Ligmengine
{
	class GraphicsManager
	{
		public:
			GLFWwindow* window;
			const char* window_name = "Ligmengine";
			int window_width = 1920;
			int window_height = 1080;
			bool window_fullscreen = false;

			void Startup();
			void Shutdown();
			void Draw();
	};
}
