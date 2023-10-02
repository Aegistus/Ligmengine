#pragma once
#include "GLFW/glfw3.h"
#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>
#include <Types.h>

namespace Ligmengine
{
	class GUIManager
	{
		public:
			bool showComponentWindow = false;
			EntityID currentlyInspecting = -1;

			void Startup();
			void Draw(WGPURenderPassEncoder renderPass);
			void Shutdown();

			void DrawHierarchyWindow();
			void DrawComponentWindow(EntityID e);
	};
}