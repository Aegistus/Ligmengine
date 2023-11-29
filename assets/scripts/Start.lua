-- This script is run automatically at startup.
-- Use it to set up your initial game state.
-- DO NOT DELETE THIS SCRIPT

print("STARTUP START")
objOne = CreateEntity("Player")

GetTransform(objOne).position = vector3:new( 100,0,.1 )
GetSpriteRenderer(objOne).sprite = GetSprite("Player")
GetScript(objOne).name = "PlayerMovement"

-- Add background
background = CreateEntity("Background")
transform = GetTransform(background)
transform.position = vector3:new(0, 0 , 1)
transform.scale = vector2:new(20, 20)
GetSpriteRenderer(background).sprite = GetSprite("Background")

-- Dash Stuff
maxDashCharges = 3
dashCharges = maxDashCharges
maxDashCooldown = 3.0
dashCooldown = maxDashCooldown

print("STARTUP DONE")
