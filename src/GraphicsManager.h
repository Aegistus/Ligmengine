#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>
#include <Types.h>

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

			WGPUInstance webGPU;
			WGPUSurface surface;
			WGPUAdapter adapter;
			WGPUDevice device;
			WGPUQueue gpuQueue;

			WGPUBuffer vertex_buffer;
			WGPUSwapChain swapchain;
			WGPUBuffer uniform_buffer;
			WGPUSampler sampler;
			WGPUShaderModule shader_module;

			void Startup();
			void Shutdown();
			void Draw();
			void RecreateSwapChain();
	};
}
