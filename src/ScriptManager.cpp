#include <ScriptManager.h>
#include <sol/sol.hpp>
#include <Engine.h>
#include <ECS.h>
#include <Components.h>
#include "spdlog/spdlog.h"
#include <Types.h>
#include <map.h>
#include <InputCode.h>
#include <functional>

#define CAT(x, y) CAT_(x, y)
#define CAT_(x, y) x ## y
#define STR(x) #x
#define COMPONENT_TEMPLATE(type) {\
	lua.set_function(CAT("Has",STR(type)), [&](EntityID e){return gEngine.ecs.HasComponent<type>(e);});\
	lua.set_function(CAT("Get",STR(type)), [&](EntityID e)->type& {return gEngine.ecs.GetComponent<type>(e);});\
	lua.set_function(CAT("Add",STR(type)), [&](EntityID e) { gEngine.ecs.AddComponent<type>(e);});\
	lua.set_function(CAT("Remove",STR(type)), [&](EntityID e) {gEngine.ecs.RemoveComponent<type>(e);});\
	lua.set_function(CAT("ForEach", STR(type)), [&](ComponentCallback callback) {gEngine.ecs.ForEach<type>(callback);});\
}

using namespace sol;

namespace Ligmengine
{
	void ScriptManager::Startup()
	{
		lua.open_libraries(lib::base, lib::math, lib::table);
		lua.script("math.randomseed(0)");
		// basic types
		lua.new_usertype<vector3>("vector3",
			constructors<vector3(), vector3(float), vector3(float, float, float)>(),
			"x", &vector3::x,
			"y", &vector3::y,
			"z", &vector3::z,
			// optional and fancy: operator overloading. see: https://github.com/ThePhD/sol2/issues/547
			meta_function::addition, sol::overload([](const vector3& v1, const vector3& v2) -> vector3 { return v1 + v2; }),
			meta_function::subtraction, sol::overload([](const vector3& v1, const vector3& v2) -> vector3 { return v1 - v2; }),
			meta_function::multiplication, sol::overload(
				[](const vector3& v1, const vector3& v2) -> vector3 { return v1 * v2; },
				[](const vector3& v1, float f) -> vector3 { return v1 * f; },
				[](float f, const vector3& v1) -> vector3 { return f * v1; }
			)
		);
		lua.new_usertype<vector2>("vector2",
			constructors<vector2(), vector2(float), vector2(float, float)>(),
			"x", &vector2::x,
			"y", &vector2::y,
			// optional and fancy: operator overloading. see: https://github.com/ThePhD/sol2/issues/547
			meta_function::addition, sol::overload([](const vector2& v1, const vector2& v2) -> vector2 { return v1 + v2; }),
			meta_function::subtraction, sol::overload([](const vector2& v1, const vector2& v2) -> vector2 { return v1 - v2; }),
			meta_function::multiplication, sol::overload(
				[](const vector2& v1, const vector2& v2) -> vector2 { return v1 * v2; },
				[](const vector2& v1, float f) -> vector2 { return v1 * f; },
				[](float f, const vector2& v1) -> vector2 { return f * v1; }
			)
		);
		// input manager functions
		lua.set_function("GetKeyDown", [&](const int inputCode) { return gEngine.input.GetKeyDown(static_cast<InputCode>(inputCode)); });
		lua.set_function("GetKeyUp", [&](const int inputCode) { return gEngine.input.GetKeyUp(static_cast<InputCode>(inputCode));  });
		lua.set_function("GetKey", [&](const int inputCode) { return gEngine.input.GetKey(static_cast<InputCode>(inputCode)); });
		// sound manager functions
		lua.set_function("PlaySound", [&](const string& name) { return gEngine.soundManager.PlaySound(name); });
		// ecs functions
		lua.set_function("CreateEntity", [&]() { return gEngine.ecs.CreateEntity(); });
		lua.set_function("DestroyEntity", [&](EntityID e) { gEngine.ecs.DestroyEntity(e); });
		// COMPONENTS
		lua.new_usertype<Transform>
		(
			"Transform",
			constructors<Transform()>(),
			"position", &Transform::position,
			"rotation", &Transform::rotation,
			"scale", &Transform::scale
		);
		lua.new_usertype<SpriteRenderer>
		(
			"SpriteRenderer",
			constructors<SpriteRenderer()>(),
			"sprite", &SpriteRenderer::sprite
		);
		lua.new_usertype<Script>
			(
				"Script",
				constructors<Script()>(),
				"name", &Script::name
			);
		// COMPONENT FUNCTIONS
		COMPONENT_TEMPLATE(Transform);
		COMPONENT_TEMPLATE(SpriteRenderer);
		COMPONENT_TEMPLATE(Script);
		
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