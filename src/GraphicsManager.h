#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <webgpu/webgpu.h>
#include <glfw3webgpu.h>
#include <Types.h>
#include <vector>
#include <Components.h>
#include <GUIManager.h>

namespace Ligmengine
{
	class GraphicsManager
	{
		public:
			GLFWwindow* window = { 0 };
			const char* window_name = "Ligmengine";
			int window_width = 1920;
			int window_height = 1080;
			bool window_fullscreen = false;

			WGPUInstance instance = { 0 };
			WGPUSurface surface = { 0 };
			WGPUAdapter adapter = { 0 };
			WGPUDevice device = { 0 };
			WGPUQueue queue = { 0 };
			WGPURenderPipeline pipeline = { 0 };

			WGPUBuffer vertex_buffer = { 0 };
			WGPUSwapChain swapchain = { 0 };
			WGPUTextureFormat swap_chain_format = { };
			WGPUBuffer uniform_buffer = { 0 };
			WGPUSampler sampler = { 0 };
			WGPUShaderModule shader_module = { 0 };

			GUIManager guiManager;

			void Startup();
			void Shutdown();
			void Draw();
			void UpdateWindowSize();
	};
}
