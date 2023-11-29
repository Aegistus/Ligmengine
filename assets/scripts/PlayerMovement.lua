-- script to control player with WASD
-- Stolen from Joe's demo for ligmengine :) thank you joe

local e = ...
moveSpeed = 50
dashSpeed = 40
rotationSpeed = 10
transform = GetTransform(e)

--for n in pairs(_G) do print(n) end

-- Dash with SPACE
if GetKeyDown(32) and dashCharges > 0 then
    if GetKey(87) then
        transform.position.y = transform.position.y + dashSpeed
    end
    if GetKey(83) then
        transform.position.y = transform.position.y - dashSpeed
    end
    if GetKey(65) then
        transform.position.x = transform.position.x - dashSpeed
    end
    if GetKey(68) then
        transform.position.x = transform.position.x + dashSpeed
    end
    dashCharges = dashCharges - 1
end

if dashCharges < maxDashCharges then
    dashCooldown = dashCooldown - GetDeltaTime()
    if dashCooldown <= 0 then
        dashCharges = dashCharges + 1
        dashCooldown = maxDashCooldown
    end
end

-- W
if GetKey(87) then
    transform.position.y = transform.position.y + moveSpeed * GetDeltaTime()
end

-- S
if GetKey(83) then
    transform.position.y = transform.position.y - moveSpeed * GetDeltaTime()
end

-- A
if GetKey(65) then
    transform.position.x = transform.position.x - moveSpeed * GetDeltaTime()
end

-- D
if GetKey(68) then
    transform.position.x = transform.position.x + moveSpeed * GetDeltaTime()
end