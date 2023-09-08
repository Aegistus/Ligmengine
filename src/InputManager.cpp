#include <Engine.h>
#include <InputManager.h>
#include <InputCode.h>
#include <unordered_map>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Ligmengine
{
	std::unordered_map<InputCode, bool> lastFrame;

	void InputManager::Startup()
	{

	}

	void InputManager::Shutdown()
	{

	}

	void InputManager::Update()
	{
		glfwPollEvents();
	}

	void InputManager::LateUpdate()
	{
		for (auto const& [key, val] : lastFrame)
		{
			lastFrame[key] = GetKey(key);
		}
	}

	bool InputManager::GetKeyDown(InputCode key)
	{
		if (!lastFrame.count(key) == 0)
		{
			lastFrame.insert({ key, GetKey(key) });
		}
		if (GetKey(key) && lastFrame[key] == false)
		{
			return true;
		}
		return false;
	}

	bool InputManager::GetKey(InputCode key)
	{
		int pressed = glfwGetKey(gEngine.graphics.window, (int)key);
		if (pressed == GLFW_PRESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputManager::GetKeyUp(InputCode key)
	{
		if (!lastFrame.count(key) == 0)
		{
			lastFrame.insert({ key, GetKey(key) });
		}
		if (!GetKey(key) && lastFrame[key] == true)
		{
			return true;
		}
		return false;
	}
}
