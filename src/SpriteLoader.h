#pragma once

#include <ResourceManager.h>
#include <unordered_map>
#include <Types.h>
#include <webgpu/webgpu.h>
#include <Components.h>

namespace Ligmengine
{
	class SpriteLoader
	{
		public:
			std::unordered_map<string, Sprite> sprites;

			bool LoadSprite(const string& name, const string& path);
			//void SetSortOrder(const string& name, float order);

			void Shutdown();
	};
}