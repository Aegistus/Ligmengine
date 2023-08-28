#include <Engine.h>
#include <InputManager.h>
#include <InputCode.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Ligmengine
{
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

	bool InputManager::GetKeyDown(InputCode key)
	{
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
		return false;
	}
}
