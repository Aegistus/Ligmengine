#pragma once
#include <InputCode.h>

namespace Ligmengine
{
	class InputManager
	{
		public:
			void Startup();
			void Shutdown();
			void Update();
			void LateUpdate();
			bool GetKeyDown(InputCode key);
			bool GetKey(InputCode key);
			bool GetKeyUp(InputCode key);
	};
}