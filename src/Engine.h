#include <GraphicsManager.h>
#include <InputManager.h>

namespace Ligmengine
{

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
