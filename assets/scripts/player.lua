-- script to control player with WASD

local e = ...
moveSpeed = 10
rotationSpeed = 10
transform = GetTransform(e)

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

if GetKey(81) then
	transform.rotation = transform.rotation + rotationSpeed * GetDeltaTime()
end

if GetKey(69) then
	transform.rotation = transform.rotation - rotationSpeed * GetDeltaTime()
end