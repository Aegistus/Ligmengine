#include <GraphicsManager.h>
#include <InputManager.h>

namespace Ligmengine
{
	static const float TIME_STEP = (1 / 60);

	class Engine
	{
		public:
			GraphicsManager graphics;
			InputManager input;

			void Startup();

			void Shutdown();

			void RunGameLoop();

	};
	
	inline Engine gEngine;

}
