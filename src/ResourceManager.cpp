#include <ResourceManager.h>
#include <filesystem>

namespace Ligmengine
{
	string ResourceManager::GetFullAssetPath(string assetPath)
	{
		return srcPath + assetPath;
	}
}