#include <iostream>
#include <Engine.h>
#include <InputCode.h>
#include <SpriteLoader.h>
#include <Sprite.h>
#include <vector>
using namespace Ligmengine;

void StartupCallback()
{
    Ligmengine::gEngine.soundManager.LoadSound("ClickSound", "sounds/ClickSound.wav");
    Ligmengine::gEngine.soundManager.LoadSound("OtherSound", "sounds/OtherSound.wav");
}

int main(int argc, const char* argv[]) {
    //std::cout << "Hello, World!\n";
    Ligmengine::gEngine.spriteLoader.LoadSprite("icon", "sprites/LigmengineIcon.png");
    std::vector<Sprite> sprites;
    sprites.push_back(Ligmengine::gEngine.spriteLoader.sprites["icon"]);
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
            gEngine.graphics.Draw(sprites);
        });

    return 0;
}
