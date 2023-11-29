
local e = ...

local transform = GetTransform(e)
local playerPos = GetTransform(playerObject).position
local displacement = transform.position - playerPos
local distanceToPlayer = displacement.x * displacement.x + displacement.y * displacement.y
distanceToPlayer = math.sqrt(distanceToPlayer)
if distanceToPlayer < enemyAttackRange then
	
end