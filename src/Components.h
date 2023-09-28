#pragma once

#include <Types.h>
#include <glfw3webgpu.h>

namespace Ligmengine
{
	struct Sprite
	{
		int width, height;
		WGPUTexture texture;
		vector3 position;
	};
}