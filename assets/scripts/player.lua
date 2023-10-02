-- script to control player with WASD

local e = ...
moveSpeed = 1
transform = GetTransform(e)

-- W
if GetKey(87) then
	transform.position.y = transform.position.y + moveSpeed
end

-- S
if GetKey(83) then
	transform.position.y = transform.position.y - moveSpeed
end

-- A
if GetKey(65) then
	transform.position.x = transform.position.x - moveSpeed
end

-- D
if GetKey(68) then
	transform.position.x = transform.position.x + moveSpeed
end
