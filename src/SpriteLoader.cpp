#include <SpriteLoader.h>
#include <Types.h>
#include <Engine.h>
#include <ResourceManager.h>
#include <Sprite.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "spdlog/spdlog.h"

namespace Ligmengine
{
	template< typename T > constexpr const T* to_ptr( const T& val ) { return &val; }
    template< typename T, std::size_t N > constexpr const T* to_ptr( const T (&&arr)[N] ) { return arr; }

	bool SpriteLoader::LoadSprite(const string& name, const string& path)
	{
		string fullPath = gEngine.resourceManager.GetFullAssetPath(path);
		int width, height, channels;
		unsigned char* data = stbi_load( fullPath.c_str(), &width, &height, &channels, 4 );
		if (data == nullptr)
		{
			spdlog::error("Failed to load sprite");
			spdlog::error(stbi_failure_reason());
		}
		// create web gpu texture
		WGPUTextureDescriptor texDesc = WGPUTextureDescriptor 
		{
			.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
			.dimension = WGPUTextureDimension_2D,
			.size = { (uint32_t)width, (uint32_t)height, 1 },
			.format = WGPUTextureFormat_RGBA8Unorm,
			.mipLevelCount = 1,
			.sampleCount = 1
		};
		spdlog::info(height);
		WGPUTexture tex = wgpuDeviceCreateTexture(gEngine.graphics.device, to_ptr(texDesc));
		// send texture to gpu
		wgpuQueueWriteTexture(
			gEngine.graphics.queue,
			to_ptr<WGPUImageCopyTexture>({ .texture = tex }),
			data,
			width * height * 4,
			to_ptr<WGPUTextureDataLayout>({ .bytesPerRow = (uint32_t)(width * 4), .rowsPerImage = (uint32_t)height }),
			to_ptr<WGPUExtent3D>({ (uint32_t)width, (uint32_t)height, 1 })
		);		
		stbi_image_free( data );
		sprites[name] = { width, height, tex, vector3(50, 50, 0) };
		// debug info
		spdlog::info(name);
		spdlog::info("width: " + std::to_string(width));
		spdlog::info("height: " + std::to_string(height));
		//spdlog::info(tex);
		return true;
	}

	void SpriteLoader::Shutdown()
	{
		sprites.clear();
	}
}
