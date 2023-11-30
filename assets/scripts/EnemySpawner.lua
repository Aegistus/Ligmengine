
local e = ...

if spawnTimer > 0 then
	spawnTimer = spawnTimer - GetDeltaTime()
else
	spawnTimer = maxSpawnTimer
	print("TEST")
	enemy = CreateEntity("Enemy")
	GetTransform(enemy).position = spawnPoint
	GetSpriteRenderer(enemy).sprite = GetSprite("Enemy")
	GetScript(enemy).name = "Enemy"
	print("TEST2")
end
