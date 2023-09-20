#pragma once

#include <Types.h>

namespace Ligmengine
{
		struct Sprite
		{
			int width, height;
			WGPUTexture texture;
			vector3 position;

			~Sprite()
			{
				if (texture != nullptr)
				{
					wgpuTextureDestroy(texture);
					wgpuTextureRelease(texture);
				}
			}
		};
}