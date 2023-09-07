#include <iostream>
#include <Engine.h>
#include <InputCode.h>
using namespace Ligmengine;

void StartupCallback()
{
    bool success = Ligmengine::gEngine.soundManager.LoadSound("ClickSound", "sounds/ClickSound.wav");
    if (!success)
    {
        std::cout << "Sound loading failed \n";
    }
}

int main(int argc, const char* argv[]) {
    //std::cout << "Hello, World!\n";
    Ligmengine::gEngine.Startup(StartupCallback, [&]()
        {
            if (gEngine.input.GetKey(InputCode::KEY_A))
            {
                std::cout << "TEST\n";
                Ligmengine::gEngine.soundManager.PlaySound("ClickSound");
            }
            if (gEngine.input.GetKey(InputCode::KEY_ESCAPE))
            {
                gEngine.quit = true;
            }
        });

    return 0;
}
