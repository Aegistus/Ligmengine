#pragma once
#include <Types.h>
#include <filesystem>

namespace Ligmengine
{

	class ResourceManager
	{
		inline static string srcPath = "./assets/";

		public:
			string GetFullAssetPath(string assetPath);
			string LoadStringFromTextFile(string fullAssetPath);
	};
}