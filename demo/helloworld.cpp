#include <iostream>
#include <Engine.h>
#include <InputCode.h>
using namespace Ligmengine;

void StartupCallback()
{
    Ligmengine::gEngine.soundManager.LoadSound("ClickSound", "sounds/ClickSound.wav");
    Ligmengine::gEngine.soundManager.LoadSound("OtherSound", "sounds/OtherSound.wav");
}

int main(int argc, const char* argv[]) {
    //std::cout << "Hello, World!\n";
    Ligmengine::gEngine.Startup(StartupCallback, [&]()
        {
            if (gEngine.input.GetKeyDown(InputCode::KEY_A))
            {
                std::cout << "TEST\n";
                Ligmengine::gEngine.soundManager.PlaySound("ClickSound");
            }
            if (gEngine.input.GetKeyUp(InputCode::KEY_A))
            {
                std::cout << "TEST 02 \n";
                Ligmengine::gEngine.soundManager.PlaySound("OtherSound");
            }
            if (gEngine.input.GetKey(InputCode::KEY_ESCAPE))
            {
                gEngine.quit = true;
            }
        });

    return 0;
}
