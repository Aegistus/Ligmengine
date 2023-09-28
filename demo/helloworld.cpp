#include <iostream>
#include <Engine.h>
#include <InputCode.h>
#include <SpriteLoader.h>
#include <Components.h>
#include <vector>
using namespace Ligmengine;

void StartupCallback(std::vector<Sprite>& sprites)
{
    Ligmengine::gEngine.soundManager.LoadSound("ClickSound", "sounds/ClickSound.wav");
    Ligmengine::gEngine.soundManager.LoadSound("OtherSound", "sounds/OtherSound.wav");
    Ligmengine::gEngine.spriteLoader.LoadSprite("icon", "sprites/LigmengineIcon.png");
    Ligmengine::gEngine.spriteLoader.LoadSprite("Gavin", "sprites/Gavin Default.png");
    Ligmengine::gEngine.spriteLoader.LoadSprite("Ollie", "sprites/Ollie Cute.png");
    sprites.push_back(Ligmengine::gEngine.spriteLoader.sprites["icon"]);
    //sprites.push_back(Ligmengine::gEngine.spriteLoader.sprites["Gavin"]);
    //sprites.push_back(Ligmengine::gEngine.spriteLoader.sprites["Ollie"]);
}

int main(int argc, const char* argv[]) {
    //std::cout << "Hello, World!\n";
    std::vector<Sprite> sprites;
    Ligmengine::gEngine.Startup([&]() {StartupCallback(sprites);}, [&]()
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
