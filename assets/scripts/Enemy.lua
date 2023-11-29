
local e = ...

-- MOVEMENT
moveSpeed = 40
transform = GetTransform(e)
playerPos = GetTransform(playerObject).position

movementVector = playerPos - transform.position
movementVector = Normalize(movementVector)

movementVector = movementVector * moveSpeed * GetDeltaTime()

movementVector = vector3:new(movementVector.x, movementVector.y, .1)

transform.position.x = transform.position.x + movementVector.x
transform.position.y = transform.position.y + movementVector.y


-- ATTACK
local transform = GetTransform(e)
local playerPos = GetTransform(playerObject).position
local displacement = transform.position - playerPos
local distanceToPlayer = displacement.x * displacement.x + displacement.y * displacement.y
distanceToPlayer = math.sqrt(distanceToPlayer)
if distanceToPlayer < enemyAttackRange then
	ReducePlayerHealth(enemyDamage * GetDeltaTime())
end
