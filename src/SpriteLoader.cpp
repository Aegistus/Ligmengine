#include <SpriteLoader.h>
#include <Types.h>
#include <Engine.h>
#include <ResourceManager.h>
#include <Sprite.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Ligmengine
{
	template< typename T > constexpr const T* to_ptr( const T& val ) { return &val; }
    template< typename T, std::size_t N > constexpr const T* to_ptr( const T (&&arr)[N] ) { return arr; }

	bool SpriteLoader::LoadSprite(const string& name, const string& path)
	{
		string fullPath = gEngine.resourceManager.GetFullAssetPath(path);
		int width, height, channels;
		unsigned char* data = stbi_load( path.c_str(), &width, &height, &channels, 4 );
		// create web gpu texture
		WGPUTextureDescriptor texDesc = WGPUTextureDescriptor{
			.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
			.dimension = WGPUTextureDimension_2D,
			.size = { (uint32_t)width, (uint32_t)height, 1 },
			.format = WGPUTextureFormat_RGBA8Unorm,
			.mipLevelCount = 1,
			.sampleCount = 1
		};
		WGPUTexture tex = wgpuDeviceCreateTexture(gEngine.graphics.device, to_ptr(texDesc));
		// send texture to gpu
		gEngine.graphics.SendTextureToGPU(data, tex, width, height);
		stbi_image_free( data );
		Sprite sprite = {width, height, tex};
		sprites.insert({name, sprite});
		return false;
	}

	void SpriteLoader::Shutdown()
	{
		sprites.clear();
	}
}
