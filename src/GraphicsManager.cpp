#include <iostream>
#include <GraphicsManager.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include <Types.h>
#include <Engine.h>
#include <vector>

using namespace glm;

namespace Ligmengine
{
	// For utility purposes
    template< typename T > constexpr const T* to_ptr( const T& val ) { return &val; }
    template< typename T, std::size_t N > constexpr const T* to_ptr( const T (&&arr)[N] ) { return arr; }

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
		instance = wgpuCreateInstance(to_ptr(WGPUInstanceDescriptor{}));
        surface = glfwGetWGPUSurface(instance, window);
        adapter = nullptr;
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
        device = nullptr;
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
        queue = wgpuDeviceGetQueue(device);

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

        uniform_buffer = wgpuDeviceCreateBuffer(device, to_ptr(WGPUBufferDescriptor{
            .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform,
            .size = sizeof(Uniforms)
        }));

        RecreateSwapChain();

        // code to make it so that the user can dynamically resize the window
        //glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        //{
        //    RecreateSwapChain();
        //});


        sampler = wgpuDeviceCreateSampler(device, to_ptr(WGPUSamplerDescriptor{
            .addressModeU = WGPUAddressMode_ClampToEdge,
            .addressModeV = WGPUAddressMode_ClampToEdge,
            .magFilter = WGPUFilterMode_Linear,
            .minFilter = WGPUFilterMode_Linear,
            .maxAnisotropy = 1
        }));
        string fullShaderPath = gEngine.resourceManager.GetFullAssetPath("shader.wgsl");
        string shaderStart = "R\"(";
        string shaderEnd = ")\"";
        const char* source = &(shaderStart + gEngine.resourceManager.LoadStringFromTextFile(fullShaderPath) + shaderEnd)[0];
        std::cout << *source << std::endl;

        WGPUShaderModuleWGSLDescriptor code_desc = {};
        code_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
        code_desc.code = source; // The shader source as a `char*`
        WGPUShaderModuleDescriptor shader_desc = {};
        shader_desc.nextInChain = &code_desc.chain;
        std::cout << "TEST01" << std::endl;

        shader_module = wgpuDeviceCreateShaderModule(device, &shader_desc);

        // declare rest of pipeline to GPU
        pipeline = wgpuDeviceCreateRenderPipeline( device, to_ptr( WGPURenderPipelineDescriptor{
    
    // Describe the vertex shader inputs
    .vertex = {
        .module = shader_module,
        .entryPoint = "vertex_shader_main",
        // Vertex attributes.
        .bufferCount = 2,
        .buffers = to_ptr<WGPUVertexBufferLayout>({
            // We have one buffer with our per-vertex position and UV data. This data never changes.
            // Note how the type, byte offset, and stride (bytes between elements) exactly matches our `vertex_buffer`.
            {
                .arrayStride = 4*sizeof(float),
                .attributeCount = 2,
                .attributes = to_ptr<WGPUVertexAttribute>({
                    // Position x,y are first.
                    {
                        .format = WGPUVertexFormat_Float32x2,
                        .offset = 0,
                        .shaderLocation = 0
                    },
                    // Texture coordinates u,v are second.
                    {
                        .format = WGPUVertexFormat_Float32x2,
                        .offset = 2*sizeof(float),
                        .shaderLocation = 1
                    }
                    })
            },
            // We will use a second buffer with our per-sprite translation and scale. This data will be set in our draw function.
            {
                .arrayStride = sizeof(InstanceData),
                // This data is per-instance. All four vertices will get the same value. Each instance of drawing the vertices will get a different value.
                // The type, byte offset, and stride (bytes between elements) exactly match the array of `InstanceData` structs we will upload in our draw function.
                .stepMode = WGPUVertexStepMode_Instance,
                .attributeCount = 2,
                .attributes = to_ptr<WGPUVertexAttribute>({
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
                    })
            }
            })
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
    .fragment = to_ptr( WGPUFragmentState{
        .module = shader_module,
        .entryPoint = "fragment_shader_main",
        
        // Our fragment shader outputs a single color value per pixel.
        .targetCount = 1,
        .targets = to_ptr<WGPUColorTargetState>({
            {
                .format = swap_chain_format,
                // The images we want to draw may have transparency, so let's turn on alpha blending with over compositing (ɑ⋅foreground + (1-ɑ)⋅background).
                // This will blend with whatever has already been drawn.
                .blend = to_ptr( WGPUBlendState{
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
                    } ),
                .writeMask = WGPUColorWriteMask_All
            }})
        } )
    } ) );
    std::cout << "TEST02" << std::endl;
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

	void GraphicsManager::Draw(std::vector<Sprite>& sprites)
	{
        WGPUBuffer instance_buffer = wgpuDeviceCreateBuffer( device,
            to_ptr<WGPUBufferDescriptor>({
                .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
                .size = sizeof(InstanceData) * sprites.size()
            })
        );
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder( device, nullptr );
        WGPUTextureView current_texture_view = wgpuSwapChainGetCurrentTextureView( swapchain );
        WGPURenderPassEncoder render_pass = wgpuCommandEncoderBeginRenderPass( encoder, to_ptr<WGPURenderPassDescriptor>({
            .colorAttachmentCount = 1,
            .colorAttachments = to_ptr<WGPURenderPassColorAttachment>({{
                .view = current_texture_view,
                .loadOp = WGPULoadOp_Clear,
                .storeOp = WGPUStoreOp_Store,
                // Choose the background color.
                .clearValue = WGPUColor{ 1.0, 1.0, 1.0, 1.0 }
                }})
            }) 
        );
        wgpuRenderPassEncoderSetPipeline( render_pass, pipeline );
        wgpuRenderPassEncoderSetVertexBuffer( render_pass, 0, vertex_buffer, 0, 4*4*sizeof(float) );
        for (int i = 0; i < sprites.size(); i++)
        {
            InstanceData d;
            if( sprites.at(i).width < sprites.at(i).height) {
                d.scale = vec2( real(sprites.at(i).width / sprites.at(i).height), 1.0 );
            } else {
                d.scale = vec2( 1.0, real(sprites.at(i).height) / sprites.at(i).width );
            }
            d.translation = sprites.at(i).position;

            wgpuQueueWriteBuffer( queue, instance_buffer, i * sizeof(InstanceData), &d, sizeof(InstanceData) );
            auto layout = wgpuRenderPipelineGetBindGroupLayout( pipeline, 0 );
                WGPUBindGroup bind_group = wgpuDeviceCreateBindGroup( device, 
                to_ptr( WGPUBindGroupDescriptor
                {
                    .layout = layout,
                    .entryCount = 3,
                    // The entries `.binding` matches what we wrote in the shader.
                    .entries = to_ptr<WGPUBindGroupEntry>({
                        {
                            .binding = 0,
                            .buffer = uniform_buffer,
                            .size = sizeof( Uniforms )
                        },
                        {
                            .binding = 1,
                            .sampler = sampler,
                        },
                        {
                            .binding = 2,
                            .textureView = wgpuTextureCreateView( sprites.at(i).texture, nullptr )
                        }
                    })
                })
            );
            wgpuBindGroupLayoutRelease( layout );
            wgpuRenderPassEncoderSetBindGroup(render_pass, 0, bind_group, 0, nullptr);
            wgpuRenderPassEncoderDraw( render_pass, 4, 1, 0, i );
            wgpuBindGroupRelease(bind_group);
        }
        wgpuRenderPassEncoderEnd( render_pass );
        WGPUCommandBuffer command = wgpuCommandEncoderFinish( encoder, nullptr );
        wgpuQueueSubmit( queue, 1, &command );
        wgpuBufferRelease(instance_buffer);
        wgpuSwapChainPresent( swapchain );

        // cleanup
        wgpuTextureViewRelease(current_texture_view);
        wgpuCommandEncoderRelease(encoder);
	}

    // called when game window is resized
    void GraphicsManager::RecreateSwapChain()
    {

        if (swapchain != nullptr)
        {
            wgpuSwapChainRelease(swapchain);
        }

        swap_chain_format = wgpuSurfaceGetPreferredFormat(surface, adapter);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        swapchain = wgpuDeviceCreateSwapChain(device, surface, to_ptr(WGPUSwapChainDescriptor{
            .usage = WGPUTextureUsage_RenderAttachment,
            .format = swap_chain_format,
            .width = (uint32_t)width,
            .height = (uint32_t)height,
        }));
        Uniforms uniforms;
        // Start with an identity matrix.
        uniforms.projection = mat4{1};
        // Scale x and y by 1/100.
        uniforms.projection[0][0] = uniforms.projection[1][1] = 1./100.;
        // Scale the long edge by an additional 1/(long/short) = short/long.
        if( width < height ) {
            uniforms.projection[1][1] *= width;
            uniforms.projection[1][1] /= height;
        } else {
            uniforms.projection[0][0] *= height;
            uniforms.projection[0][0] /= width;
        }
        wgpuQueueWriteBuffer( queue, uniform_buffer, 0, &uniforms, sizeof(Uniforms) );
    }

    void GraphicsManager::SendTextureToGPU(unsigned char *data, WGPUTexture tex, int width, int height)
    {
        wgpuQueueWriteTexture(
            queue,
            to_ptr<WGPUImageCopyTexture>({ .texture = tex }),
            data,
            width * height * 4,
            to_ptr<WGPUTextureDataLayout>({ .bytesPerRow = (uint32_t)(width*4), .rowsPerImage = (uint32_t)height }),
            to_ptr<WGPUExtent3D>( { (uint32_t)width, (uint32_t)height, 1 } )
        );
    }
}
