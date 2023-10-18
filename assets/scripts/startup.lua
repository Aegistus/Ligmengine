-- create entities

print("STARTUP START")
objOne = CreateEntity("Player")

GetTransform(objOne).position = vector3:new( 100,0,1 )
GetSpriteRenderer(objOne).sprite = GetSprite("Player")
GetRigidBody(objOne).useGravity = true;

--LoadScript("Player", "scripts/player.lua")
GetScript(objTwo).name = "Player"
print("STARTUP DONE")