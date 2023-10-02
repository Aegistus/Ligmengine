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

    gEngine.ecs.GetComponent<EntityData>(objOne).name = "Logo";
    gEngine.ecs.GetComponent<EntityData>(objTwo).name = "Player";
    gEngine.ecs.GetComponent<EntityData>(objThree).name = "Ollie";

    gEngine.ecs.GetComponent<Transform>(objOne).position = { 100,0,1 };
    gEngine.ecs.GetComponent<Transform>(objTwo).position = { 50,0,.5 };
    gEngine.ecs.GetComponent<Transform>(objThree).position = { 0,0,0 };

    SpriteRenderer s1;
    s1.sprite = gEngine.spriteLoader.GetSprite("Icon");
    SpriteRenderer s2;
    s2.sprite = gEngine.spriteLoader.GetSprite("Gavin");
    SpriteRenderer s3;
    s3.sprite = gEngine.spriteLoader.GetSprite("Ollie");
    gEngine.ecs.GetComponent<SpriteRenderer>(objOne).sprite = gEngine.spriteLoader.GetSprite("Icon");
    gEngine.ecs.GetComponent<SpriteRenderer>(objTwo).sprite = gEngine.spriteLoader.GetSprite("Gavin");
    gEngine.ecs.GetComponent<SpriteRenderer>(objThree).sprite = gEngine.spriteLoader.GetSprite("Ollie");

    gEngine.scriptManager.LoadScript("Player", "scripts/player.lua");
    gEngine.ecs.GetComponent<Script>(objTwo).name = "Player";
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
