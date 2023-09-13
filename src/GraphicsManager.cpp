#include <iostream>
#include <GraphicsManager.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include <Types.h>
#include <Engine.h>

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
        };

        struct Uniforms
        {
            matrix4x4 projection;
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

        RecreateSwapChain();
        // code to make it so that the user can properly resize the window
        //glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        //{
        //    RecreateSwapChain();
        //});

        uniform_buffer = wgpuDeviceCreateBuffer(device, to_ptr(WGPUBufferDescriptor{
            .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform,
            .size = sizeof(Uniforms)
        }));
        sampler = wgpuDeviceCreateSampler(device, to_ptr(WGPUSamplerDescriptor{
            .addressModeU = WGPUAddressMode_ClampToEdge,
            .addressModeV = WGPUAddressMode_ClampToEdge,
            .magFilter = WGPUFilterMode_Linear,
            .minFilter = WGPUFilterMode_Linear,
            .maxAnisotropy = 1
        }));
        const char* source = &gEngine.resourceManager.GetFullAssetPath("shader.wgsl")[0];
        WGPUShaderModuleWGSLDescriptor code_desc = {};
        code_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
        code_desc.code = source; // The shader source as a `char*`
        WGPUShaderModuleDescriptor shader_desc = {};
        shader_desc.nextInChain = &code_desc.chain;
        shader_module = wgpuDeviceCreateShaderModule(device, &shader_desc);

        // declare rest of pipeline to GPU
        WGPURenderPipeline pipeline = wgpuDeviceCreateRenderPipeline(device, to_ptr(WGPURenderPipelineDescriptor{

            // Describe the vertex shader inputs
            .vertex = {
                .module = shader_module,
                .entryPoint = "vertex_shader_main",
                // Vertex attributes.
                .bufferCount = 2,
                .buffers = (WGPUVertexBufferLayout[]){
                // We have one buffer with our per-vertex position and UV data. This data never changes.
                // Note how the type, byte offset, and stride (bytes between elements) exactly matches our `vertex_buffer`.
                {
                    .arrayStride = 4 * sizeof(float),
                    .attributeCount = 2,
                    .attributes = (WGPUVertexAttribute[]){
                // Position x,y are first.
                {
                    .format = WGPUVertexFormat_Float32x2,
                    .offset = 0,
                    .shaderLocation = 0
                },
            // Texture coordinates u,v are second.
            {
                .format = WGPUVertexFormat_Float32x2,
                .offset = 2 * sizeof(float),
                .shaderLocation = 1
            }
            }
            },
            // We will use a second buffer with our per-sprite translation and scale. This data will be set in our draw function.
            {
                .arrayStride = sizeof(InstanceData),
                // This data is per-instance. All four vertices will get the same value. Each instance of drawing the vertices will get a different value.
                // The type, byte offset, and stride (bytes between elements) exactly match the array of `InstanceData` structs we will upload in our draw function.
                .stepMode = WGPUVertexStepMode_Instance,
                .attributeCount = 2,
                .attributes = (WGPUVertexAttribute[]){
                // Translation as a 3D vector.
                {
                    .format = WGPUVertexFormat_Float32x3,
                    .offset = offsetof(InstanceData, translation),
                    .shaderLocation = 2
                },
            // Scale as a 2D vector for non-uniform scaling.
            {
                .format = WGPUVertexFormat_Float32x2,
                .offset = offsetof(InstanceData, scale),
                .shaderLocation = 3
            }
            }
                }
                }
            },

            // Interpret our 4 vertices as a triangle strip
            .primitive = WGPUPrimitiveState{
                .topology = WGPUPrimitiveTopology_TriangleStrip,
                },

                // No multi-sampling (1 sample per pixel, all bits on).
                .multisample = WGPUMultisampleState{
                    .count = 1,
                    .mask = ~0u
                    },

            // Describe the fragment shader and its output
            .fragment = to_ptr(WGPUFragmentState{
                .module = shader_module,
                .entryPoint = "fragment_shader_main",

                // Our fragment shader outputs a single color value per pixel.
                .targetCount = 1,
                .targets = (WGPUColorTargetState[]){
                    {
                        .format = swap_chain_format,
                        // The images we want to draw may have transparency, so let's turn on alpha blending with over compositing (ɑ⋅foreground + (1-ɑ)⋅background).
                        // This will blend with whatever has already been drawn.
                        .blend = to_ptr(WGPUBlendState{
                    // Over blending for color
                    .color = {
                        .operation = WGPUBlendOperation_Add,
                        .srcFactor = WGPUBlendFactor_SrcAlpha,
                        .dstFactor = WGPUBlendFactor_OneMinusSrcAlpha
                        },
                            // Leave destination alpha alone
                            .alpha = {
                                .operation = WGPUBlendOperation_Add,
                                .srcFactor = WGPUBlendFactor_Zero,
                                .dstFactor = WGPUBlendFactor_One
                                }
                            }),
                        .writeMask = WGPUColorWriteMask_All
                    }}
                })
            }));
    }

	void GraphicsManager::Shutdown()
	{
        wgpuShaderModuleRelease(shader_module);
        wgpuSamplerRelease(sampler);
        wgpuBufferRelease(uniform_buffer);
        wgpuSwapChainRelease(swapchain);
        wgpuBufferDestroy(vertex_buffer);
        wgpuBufferRelease(vertex_buffer);
        wgpuDeviceRelease(device);
        wgpuAdapterRelease(adapter);
		glfwTerminate();
	}

	void GraphicsManager::Draw()
	{


	}

    // called when game window is resized
    void GraphicsManager::RecreateSwapChain()
    {
        if (swapchain != nullptr)
        {
            wgpuSwapChainRelease(swapchain);
        }
        WGPUTextureFormat swap_chain_format = wgpuSurfaceGetPreferredFormat(surface, adapter);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        swapchain = wgpuDeviceCreateSwapChain(device, surface, to_ptr(WGPUSwapChainDescriptor{
            .usage = WGPUTextureUsage_RenderAttachment,
            .format = swap_chain_format,
            .width = (uint32_t)width,
            .height = (uint32_t)height,
        }));
    }
}
