#pragma once
#include "soloud.h"
#include "soloud_wav.h"
#include <Types.h>
#include <unordered_map>

namespace Ligmengine
{
	class SoundManager
	{
		private:
			std::unordered_map<string, SoLoud::Wav> soundMap;

		public:
			SoLoud::Soloud soLoud;

			void Startup();
			void Shutdown();
			bool LoadSound(const string& name, const string& path);
			void PlaySound(const string& name);
	};
}