#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <Types.h>
#include <unordered_map>

using namespace sol;
using namespace std;

namespace Ligmengine
{
	class ScriptManager
	{
		public:
			state lua;
			unordered_map<string, load_result> loadedScripts;

			void Startup();
			bool LoadScript(const string& name, const string& path);
			bool RunScript(const string& name);
			bool LoadAndRunScript(const string& path);
			void LoadAllScriptsInFolder(const string& path);
			void Update();
		private:
			void CreateInputCodeEnum();
	};
}