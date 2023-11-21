-- This script is run automatically at startup.
-- Use it to load all of your assets.
-- DO NOT DELETE THIS SCRIPT
print("LOADING ASSETS")

--load sounds
LoadSound("ClickSound", "sounds/SFX_SelectSound_Quinn.ogg")
LoadSound("OtherSound", "sounds/OtherSound.wav")

--load sprites
LoadSprite("Player", "sprites/Princess.png")
LoadSprite("Background", "sprites/HellLevelBackground.png")

--load scripts
--LoadScript("Player", "scripts/Player.lua")