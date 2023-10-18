-- script to control player with WASD

local e = ...
moveSpeed = 10
rotationSpeed = 50
transform = GetTransform(e)
w = 87
s = 83
a = 65
d = 68

-- W
if GetKey(w) then
	transform.position.y = transform.position.y + moveSpeed * GetDeltaTime()
end

-- S
if GetKey(s) then
	transform.position.y = transform.position.y - moveSpeed * GetDeltaTime()
end

-- A
if GetKey(a) then
	transform.position.x = transform.position.x - moveSpeed * GetDeltaTime()
end

-- D
if GetKey(d) then
	transform.position.x = transform.position.x + moveSpeed * GetDeltaTime()
end
