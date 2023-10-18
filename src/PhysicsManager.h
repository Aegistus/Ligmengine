#pragma once
#include "Components.h"
#include "Types.h"

namespace Ligmengine
{
	class PhysicsManager
	{
		public:
			vector2 gravity = { 0, -9.8 };

			void Startup();
			void Update();
			void Shutdown();
	};
}