-- This script is run automatically at startup.
-- Use it to set up your initial game state.
-- DO NOT DELETE THIS SCRIPT

print("STARTUP START")
objOne = CreateEntity("Player")

GetTransform(objOne).position = vector3:new( 100,0,1 )
GetSpriteRenderer(objOne).sprite = GetSprite("Player")
GetRigidBody(objOne).useGravity = true;

GetScript(objTwo).name = "Player"
print("STARTUP DONE")