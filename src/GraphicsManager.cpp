#include <iostream>
#include <GraphicsManager.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include <Types.h>

using namespace glm;

namespace Ligmengine
{
	// For utility purposes
	template< typename T > const T* to_ptr(const T& val) { return &val; }

    namespace
    {
        struct InstanceData
        {
            vector3 translation;
            vector2 scale;
            // rotation?
        };
    }

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
		// WebGPU setup
		WGPUInstance instance = wgpuCreateInstance(to_ptr(WGPUInstanceDescriptor{}));
        WGPUSurface surface = glfwGetWGPUSurface(instance, window);

        WGPUAdapter adapter = nullptr;
        wgpuInstanceRequestAdapter(
            instance,
            to_ptr(WGPURequestAdapterOptions{ .compatibleSurface = surface }),
            [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* adapter_ptr) {
                if (status != WGPURequestAdapterStatus_Success) {
                    std::cerr << "Failed to get a WebGPU adapter: " << message << std::endl;
                    glfwTerminate();
                }

                *static_cast<WGPUAdapter*>(adapter_ptr) = adapter;
            },
            &(adapter)
                );

        WGPUDevice device = nullptr;
        wgpuAdapterRequestDevice(
            adapter,
            nullptr,
            [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* device_ptr) {
                if (status != WGPURequestDeviceStatus_Success) {
                    std::cerr << "Failed to get a WebGPU device: " << message << std::endl;
                    glfwTerminate();
                }

                *static_cast<WGPUDevice*>(device_ptr) = device;
            },
            &(device)
        );

        // An error callback to help with debugging
        wgpuDeviceSetUncapturedErrorCallback(
            device,
            [](WGPUErrorType type, char const* message, void*) {
                std::cerr << "WebGPU uncaptured error type " << type << " with message: " << message << std::endl;
            },
            nullptr
        );

        WGPUQueue queue = wgpuDeviceGetQueue(device);

        // A vertex buffer containing a textured square.
        const struct {
            // position
            vector2 position;
            // texcoords
            vector2 uv;
        } vertices[] = {
            // position       // texcoords
          { {-1.0f,  -1.0f},    {0.0f,  1.0f }},
          { {1.0f,  -1.0f},    {1.0f,  1.0f }},
          { {-1.0f,   1.0f},    {0.0f,  0.0f }},
          {  {1.0f,   1.0f},    {1.0f,  0.0f }},
        };
        vertex_buffer = wgpuDeviceCreateBuffer(device, to_ptr(WGPUBufferDescriptor{
            .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
            .size = sizeof(vertices)
            }));
        wgpuQueueWriteBuffer(queue, vertex_buffer, 0, vertices, sizeof(vertices));
    }

	void GraphicsManager::Shutdown()
	{
        wgpuBufferDestroy(vertex_buffer);
        wgpuBufferRelease(vertex_buffer);
        wgpuDeviceRelease(device);
        wgpuAdapterRelease(adapter);
		glfwTerminate();
	}

	void GraphicsManager::Draw()
	{


	}
}
