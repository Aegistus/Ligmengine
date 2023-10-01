#include <ScriptManager.h>
#include <sol/sol.hpp>
#include <Engine.h>
#include <ECS.h>
#include <Components.h>
#include "spdlog/spdlog.h"

using namespace sol;

namespace Ligmengine
{
	void ScriptManager::Startup()
	{
		lua.open_libraries(lib::base, lib::math, lib::table);
		lua.script("math.randomseed(0)");
		// input manager functions
		lua.set_function("GetKeyDown", [&](const InputCode inputCode) { return gEngine.input.GetKeyDown(inputCode); });
		lua.set_function("GetKeyUp", [&](const InputCode inputCode) { return gEngine.input.GetKeyUp(inputCode);  });
		lua.set_function("GetKey", [&](const InputCode inputCode) { return gEngine.input.GetKey(inputCode); });
		// sound manager functions
		lua.set_function("PlaySound", [&](const string& name) { return gEngine.soundManager.PlaySound(name); });
		// ecs functions
		lua.set_function("CreateEntity", [&]() { return gEngine.ecs.CreateEntity(); });
		lua.set_function("DestroyEntity", [&]() { gEngine.ecs.DestroyEntity(); });
		// COMPONENT FUNCTIONS
		// transform
		lua.set_function("HasTransform", [&](EntityID e) {return gEngine.ecs.HasComponent<Transform>(e); });
		lua.set_function("GetTransform", [&](EntityID e) {return gEngine.ecs.GetComponent<Transform>(e); });
		lua.set_function("AddTransform", [&](EntityID e) { gEngine.ecs.AddComponent<Transform>(e); });
		lua.set_function("RemoveTransform", [&](EntityID e) {gEngine.ecs.RemoveComponent<Transform>(e); });
		// sprite renderer

		// application functions
		lua.set_function("Quit", [&]() { gEngine.quit = true; });
	}

	bool ScriptManager::LoadScript(const string& name, const string& path)
	{
		string fullPath = gEngine.resourceManager.GetFullAssetPath(path);
		loadedScripts[name] = lua.load_file(fullPath);
		if (!loadedScripts[name].valid()) {
			sol::error err = loadedScripts[name];
			spdlog::error("Failed to load script: ");
			spdlog::error(err.what());
			return false;
		}
		return true;
	}

	void ScriptManager::Update()
	{
		gEngine.ecs.ForEach<Script>([&](EntityID entity)
			{
				Script& script = gEngine.ecs.GetComponent<Script>(entity);
				if (!loadedScripts[script.name].valid())
				{
					sol::error err = loadedScripts[script.name];
					spdlog::error("Attempting to run invalid script");
					spdlog::error("    Entity: {}", entity);
					spdlog::error("	   Script name: {}", script.name);
				}
				else
				{
					loadedScripts[script.name](entity);
				}
			}
		);
	}
}