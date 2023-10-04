#include <ResourceManager.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Types.h>
#include "spdlog/spdlog.h"

using std::ifstream; using std::ostringstream;

namespace Ligmengine
{
	string ResourceManager::GetFullAssetPath(string assetPath)
	{
		return srcPath + assetPath;
	}

	string ResourceManager::LoadStringFromTextFile(string fullAssetPath)
	{
		ifstream input_file(fullAssetPath);
		string str = string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
		spdlog::info(str);
		return str;
	}
}