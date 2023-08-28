
namespace Ligmengine
{
	class GraphicsManager
	{
		public:
			const char* window_name = "Ligmengine";
			int window_width = 1920;
			int window_height = 1080;
			bool window_fullscreen = false;

			void Startup();
			void Shutdown();
			void Draw();
	};
}
