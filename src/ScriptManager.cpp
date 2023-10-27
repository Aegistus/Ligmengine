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
#include <filesystem>

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
using namespace std::filesystem;

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
		lua.set_function("Log", [&](string content) { spdlog::info(content); });
		lua.set_function("GetDeltaTime", [&]() { return gEngine.deltaTime; });
		// script manager functions
		lua.set_function("LoadScript", [&](const string& name, const string& path) { return LoadScript(name, path); });
		lua.set_function("RunScript", [&](const string& name) { return RunScript(name); });

		// input manager functions
		lua.set_function("GetKeyDown", [&](const int inputCode) { return gEngine.input.GetKeyDown(static_cast<InputCode>(inputCode)); });
		lua.set_function("GetKeyUp", [&](const int inputCode) { return gEngine.input.GetKeyUp(static_cast<InputCode>(inputCode));  });
		lua.set_function("GetKey", [&](const int inputCode) { return gEngine.input.GetKey(static_cast<InputCode>(inputCode)); });

		// sprite loader
		lua.set_function("LoadSprite", [&](const string& name, const string& path) { return gEngine.spriteLoader.LoadSprite(name, path); });
		lua.set_function("GetSprite", [&](const string& name) { return gEngine.spriteLoader.GetSprite(name); });

		// sound manager functions
		lua.set_function("LoadSound", [&](const string& name, const string& path) { return gEngine.soundManager.LoadSound(name, path); });
		lua.set_function("PlaySound", [&](const string& name) { return gEngine.soundManager.PlaySound(name); });

		// ecs functions
		lua.set_function("CreateEntity", [&]() { return gEngine.ecs.CreateEntity(); });
		lua.set_function("CreateEntity", [&](string name) { return gEngine.ecs.CreateEntity(name); });
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
		lua.new_usertype<RigidBody>
			(
				"RigidBody",
				constructors<RigidBody()>(),
				"useGravity", &RigidBody::useGravity,
				"velocity", &RigidBody::velocity
			);
		// COMPONENT FUNCTIONS
		COMPONENT_TEMPLATE(Transform);
		COMPONENT_TEMPLATE(SpriteRenderer);
		COMPONENT_TEMPLATE(Script);
		COMPONENT_TEMPLATE(RigidBody);
		
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

	bool ScriptManager::RunScript(const string& name)
	{
		if (!loadedScripts[name].valid())
		{
			sol::error err = loadedScripts[name];
			spdlog::error("Attempting to run invalid script");
			spdlog::error("	   Script name: {}", name);
			return false;
		}
		else
		{
			loadedScripts[name]();
			return true;
		}
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

	void ScriptManager::CreateInputCodeEnum()
	{
		lua.new_enum
		(
			"Input",
			InputCode::KEY_UNKNOWN, "UNKNOWN",
			InputCode::KEY_SPACE, "SPACE",
			InputCode::KEY_APOSTROPHE, "APOSTROPHE",
			InputCode::KEY_COMMA, "COMMA",
			InputCode::KEY_MINUS, "MINUS",
			InputCode::KEY_PERIOD, "PERIOD",
			InputCode::KEY_SLASH, "SLASH",
			InputCode::KEY_0, "0",
			InputCode::KEY_1, "1",
			InputCode::KEY_2, "2",
			InputCode::KEY_3, "3",
			InputCode::KEY_4, "4",
			InputCode::KEY_5, "5",
			InputCode::KEY_6, "6",
			InputCode::KEY_7, "7",
			InputCode::KEY_8, "8",
			InputCode::KEY_9, "9",
			InputCode::KEY_SEMICOLON, "SEMICOLON",
			InputCode::KEY_EQUAL, "EQUAL",
			InputCode::KEY_A, "A",
			InputCode::KEY_B, "B",
			InputCode::KEY_C, "C",
			InputCode::KEY_D, "D",
			InputCode::KEY_E, "E",
			InputCode::KEY_F, "F",
			InputCode::KEY_G, "G",
			InputCode::KEY_H, "H",
			InputCode::KEY_I, "I",
			InputCode::KEY_J, "J",
			InputCode::KEY_K, "K",
			InputCode::KEY_L, "L",
			InputCode::KEY_M, "M",
			InputCode::KEY_N, "N",
			InputCode::KEY_O, "O",
			InputCode::KEY_P, "P",
			InputCode::KEY_Q, "Q",
			InputCode::KEY_R, "R",
			InputCode::KEY_S, "S",
			InputCode::KEY_T, "T",
			InputCode::KEY_U, "U",
			InputCode::KEY_V, "V",
			InputCode::KEY_W, "W",
			InputCode::KEY_X, "X",
			InputCode::KEY_Y, "Y",
			InputCode::KEY_Z, "Z",
			InputCode::KEY_LEFT_BRACKET, "LEFT_BRACKET",
			InputCode::KEY_BACKSLASH, "BACKSLASH",
			InputCode::KEY_RIGHT_BRACKET, "RIGHT_BRACKET",
			InputCode::KEY_GRAVE_ACCENT, "GRAVE_ACCENT",
			InputCode::KEY_WORLD_1, "WORLD_1",
			InputCode::KEY_WORLD_2, "WORLD_2",
			InputCode::KEY_ESCAPE, "ESCAPE",
			InputCode::KEY_ENTER, "ENTER",
			InputCode::KEY_TAB, "TAB",
			InputCode::KEY_BACKSPACE, "BACKSPACE",
			InputCode::KEY_INSERT, "INSERT",
			InputCode::KEY_DELETE, "DELETE",
			InputCode::KEY_RIGHT, "RIGHT",
			InputCode::KEY_LEFT, "LEFT",
			InputCode::KEY_DOWN, "DOWN",
			InputCode::KEY_UP, "UP",
			InputCode::KEY_PAGE_UP, "PAGE_UP",
			InputCode::KEY_PAGE_DOWN, "PAGE_DOWN",
			InputCode::KEY_HOME, "HOME",
			InputCode::KEY_END, "END",
			InputCode::KEY_CAPS_LOCK, "CAPS_LOCK",
			InputCode::KEY_SCROLL_LOCK, "SCROLL_LOCK",
			InputCode::KEY_NUM_LOCK, "NUM_LOCK",
			InputCode::KEY_PRINT_SCREEN, "PRINT_SCREEN",
			InputCode::KEY_PAUSE, "PAUSE",
			InputCode::KEY_F1, "F1",
			InputCode::KEY_F2, "F2",
			InputCode::KEY_F3, "F3",
			InputCode::KEY_F4, "F4",
			InputCode::KEY_F5, "F5",
			InputCode::KEY_F6, "F6",
			InputCode::KEY_F7, "F7",
			InputCode::KEY_F8, "F8",
			InputCode::KEY_F9, "F9",
			InputCode::KEY_F10, "F10",
			InputCode::KEY_F11, "F11",
			InputCode::KEY_F12, "F12",
			InputCode::KEY_F13, "F13",
			InputCode::KEY_F14, "F14",
			InputCode::KEY_F15, "F15",
			InputCode::KEY_F16, "F16",
			InputCode::KEY_F17, "F17",
			InputCode::KEY_F18, "F18",
			InputCode::KEY_F19, "F19",
			InputCode::KEY_F20, "F20",
			InputCode::KEY_F21, "F21",
			InputCode::KEY_F22, "F22",
			InputCode::KEY_F23, "F23",
			InputCode::KEY_F24, "F24",
			InputCode::KEY_F25, "F25",
			InputCode::KEY_KP_0, "KP_0",
			InputCode::KEY_KP_1, "KP_1",
			InputCode::KEY_KP_2, "KP_2",
			InputCode::KEY_KP_3, "KP_3",
			InputCode::KEY_KP_4, "KP_4",
			InputCode::KEY_KP_5, "KP_5",
			InputCode::KEY_KP_6, "KP_6",
			InputCode::KEY_KP_7, "KP_7",
			InputCode::KEY_KP_8, "KP_8",
			InputCode::KEY_KP_9, "KP_9",
			InputCode::KEY_KP_DECIMAL, "KP_DECIMAL",
			InputCode::KEY_KP_DIVIDE, "KP_DIVIDE",
			InputCode::KEY_KP_MULTIPLY, "KP_MULTIPLY",
			InputCode::KEY_KP_SUBTRACT, "KP_SUBTRACT",
			InputCode::KEY_KP_ADD, "KP_ADD",
			InputCode::KEY_KP_ENTER, "KP_ENTER",
			InputCode::KEY_KP_EQUAL, "KP_EQUAL",
			InputCode::KEY_LEFT_SHIFT, "LEFT_SHIFT",
			InputCode::KEY_LEFT_CONTROL, "LEFT_CONTROL",
			InputCode::KEY_LEFT_ALT, "LEFT_ALT",
			InputCode::KEY_LEFT_SUPER, "LEFT_SUPER",
			InputCode::KEY_RIGHT_SHIFT, "RIGHT_SHIFT",
			InputCode::KEY_RIGHT_CONTROL, "RIGHT_CONTROL",
			InputCode::KEY_RIGHT_ALT, "RIGHT_ALT",
			InputCode::KEY_RIGHT_SUPER, "RIGHT_SUPER",
			InputCode::KEY_MENU, "MENU",
			InputCode::KEY_LAST, "LAST"
		);
	}
}