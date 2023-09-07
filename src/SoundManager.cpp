#include <SoundManager.h>
#include <Engine.h>
#include "spdlog/spdlog.h"

namespace Ligmengine
{
	void SoundManager::Startup()
	{
		this->soLoud.init();
	}

	void SoundManager::Shutdown()
	{
		this->soLoud.deinit();
	}

	bool SoundManager::LoadSound(const string& name, const string& path)
	{
		string fullPath = gEngine.resourceManager.GetFullAssetPath(path);
		SoLoud::Wav soundWav;
		soundMap.insert({ name, soundWav });
		soundMap[name].load(fullPath.c_str());
		return false;
	}

	void SoundManager::PlaySound(const string& name)
	{
		soLoud.play(soundMap[name]);
		spdlog::info("Playing Sound");
	}

}

