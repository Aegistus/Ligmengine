-- This script is run automatically at startup.
-- Use it to load all of your assets.
-- DO NOT DELETE THIS SCRIPT
print("LOADING ASSETS")

--load sounds
LoadSound("ClickSound", "sounds/ClickSound.wav");
LoadSound("OtherSound", "sounds/OtherSound.wav");

--load sprites
LoadSprite("Player", "sprites/Player.png");

--load scripts
LoadScript("Player", "scripts/player.lua");