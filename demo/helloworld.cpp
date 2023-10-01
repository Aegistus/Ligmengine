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

    EntityID objOne = gEngine.ecs.CreateEntity();
    EntityID objTwo = gEngine.ecs.CreateEntity();
    EntityID objThree = gEngine.ecs.CreateEntity();

    gEngine.ecs.AddComponent<Transform>(objOne, Transform({ 0,0,0 }));
    gEngine.ecs.AddComponent<Transform>(objTwo, Transform({ 0,0,.5 }));
    gEngine.ecs.AddComponent<Transform>(objThree, Transform({ 0,0,1 }));

    SpriteRenderer s1;
    s1.sprite = gEngine.spriteLoader.GetSprite("Icon");
    SpriteRenderer s2;
    s2.sprite = gEngine.spriteLoader.GetSprite("Gavin");
    SpriteRenderer s3;
    s3.sprite = gEngine.spriteLoader.GetSprite("Ollie");
    gEngine.ecs.AddComponent<SpriteRenderer>(objOne, s1);
    gEngine.ecs.AddComponent<SpriteRenderer>(objTwo, s2);
    gEngine.ecs.AddComponent<SpriteRenderer>(objThree, s3);
}

int main(int argc, const char* argv[]) {
    //std::cout << "Hello, World!\n";
    gEngine.Startup([&]() {StartupCallback();}, [&]()
        {
            if (gEngine.input.GetKeyDown(InputCode::KEY_A))
            {
                std::cout << "TEST\n";
                gEngine.soundManager.PlaySound("ClickSound");
            }
            if (gEngine.input.GetKeyUp(InputCode::KEY_A))
            {
                std::cout << "TEST 02 \n";
                gEngine.soundManager.PlaySound("OtherSound");
            }
            if (gEngine.input.GetKey(InputCode::KEY_ESCAPE))
            {
                gEngine.quit = true;
            }
        });

    return 0;
}
