#include "Engine.h"
#include <iostream>

using namespace Ligmengine;

void LoadAssets();

int main(int argc, const char* argv[])
{
	cout << "Starting Gorilla Grind" << endl;
    gEngine.Startup(
        // start callback
        [&]()
        {
            LoadAssets();
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

void LoadAssets()
{
    // load sounds
    gEngine.soundManager.LoadSound("ClickSound", "sounds/ClickSound.wav");
    gEngine.soundManager.LoadSound("OtherSound", "sounds/OtherSound.wav");

    // load sprites
    gEngine.spriteLoader.LoadSprite("Player", "sprites/Player.png");

    // load scripts
    gEngine.scriptManager.LoadScript("Startup", "scripts/startup.lua");
    gEngine.scriptManager.LoadScript("Player", "scripts/player.lua");
    gEngine.scriptManager.loadedScripts["Startup"]();
}