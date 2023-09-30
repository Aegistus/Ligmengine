#pragma once
#include <Types.h>
#include <glfw3webgpu.h>

namespace Ligmengine
{
	/// <summary>
	/// Sprite Renderer component. Contains a sprite that can be rendered by the Graphics System
	/// </summary>
	struct SpriteRenderer
	{
		Sprite* sprite;
	};

	struct Transform
	{
		vector3 position;
		vector3 rotation;
		vector3 scale;
	};
}