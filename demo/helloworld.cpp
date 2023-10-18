#include <iostream>
#include <Engine.h>
#include <InputCode.h>
#include <SpriteLoader.h>
#include <vector>
#include <ECS.h>
#include <Types.h>
#include <Components.h>

using namespace Ligmengine;
using namespace std;

void StartupCallback()
{
    // load sounds
    gEngine.soundManager.LoadSound("ClickSound", "sounds/ClickSound.wav");
    gEngine.soundManager.LoadSound("OtherSound", "sounds/OtherSound.wav");
    // load sprites
    gEngine.spriteLoader.LoadSprite("Icon", "sprites/LigmengineIcon.png");
    gEngine.spriteLoader.LoadSprite("Gavin", "sprites/GavinDefault.png");
    gEngine.spriteLoader.LoadSprite("Ollie", "sprites/OllieCute.png");

    gEngine.scriptManager.LoadScript("Startup", "scripts/startup.lua");
    gEngine.scriptManager.LoadScript("Player", "scripts/player.lua");
    gEngine.scriptManager.loadedScripts["Startup"]();
}

int main(int argc, const char* argv[]) {
    //std::cout << "Hello, World!\n";
    gEngine.Startup([&]() {StartupCallback();}, [&]()
        {
            //if (gEngine.input.GetKeyDown(InputCode::KEY_A))
            //{
            //    std::cout << "TEST\n";
            //    gEngine.soundManager.PlaySound("ClickSound");
            //}
            //if (gEngine.input.GetKeyUp(InputCode::KEY_A))
            //{
            //    std::cout << "TEST 02 \n";
            //    gEngine.soundManager.PlaySound("OtherSound");
            //}
            if (gEngine.input.GetKey(InputCode::KEY_ESCAPE))
            {
                gEngine.quit = true;
            }
        });

    return 0;
}
