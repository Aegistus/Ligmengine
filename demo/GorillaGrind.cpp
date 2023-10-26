#include "Engine.h"
#include <iostream>

using namespace Ligmengine;

int main(int argc, const char* argv[])
{
	cout << "Starting Gorilla Grind" << endl;
    gEngine.Startup(
        // start callback
        [&]()
        {
            
        },
        // update callback
        [&]()
        {
            if (gEngine.input.GetKey(InputCode::KEY_ESCAPE))
            {
                gEngine.quit = true;
            }
        }
    );
	return 0;
}