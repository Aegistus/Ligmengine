-- This script is run automatically at startup.
-- Use it to set up your initial game state.
-- DO NOT DELETE THIS SCRIPT

print("STARTUP START")
playerObject = CreateEntity("Player")

GetTransform(playerObject).position = vector3:new( 100,0,.1 )
GetSpriteRenderer(playerObject).sprite = GetSprite("Player")
GetScript(playerObject).name = "Player"

-- Add background
background = CreateEntity("Background")
transform = GetTransform(background)
transform.position = vector3:new(0, 0 , 1)
transform.scale = vector2:new(20, 20)
GetSpriteRenderer(background).sprite = GetSprite("Background")

-- ADD HEALTH BAR
healthBar = CreateEntity("Health Bar")
healthBarTransform = GetTransform(healthBar)
healthBarTransform.position = vector3:new(0, 90, .99)
healthBarTransform.scale = vector2:new(5, 5)
GetSpriteRenderer(healthBar).sprite = GetSprite("Health Bar")

-- ADD ENEMY SPAWNER
enemySpawner = CreateEntity("Enemy Spawner")
GetScript(enemySpawner).name = "EnemySpawner"

-- HEALTH BAR DATA
healthBarStartWidth = healthBarTransform.scale.x

-- Dash Stuff
maxDashCharges = 3
dashCharges = maxDashCharges
maxDashCooldown = 2.0
dashCooldown = maxDashCooldown
dashSpeed = 40

-- PLAYER Stats
maxPlayerHealth = 100
currentPlayerHealth = maxPlayerHealth

-- Enemy Stats
enemySpawnCount = 3
enemyAttackRange = 10
enemyDamage = 10

-- ENEMY SPAWNING
--spawnPoints = {	vector3:new(50, 50, .2), vector3:new(-50, 50, .2), vector3:new(50, -50, .2), vector3:new(-50,-50,.2) }
spawnPoint = vector3:new(50,50,.2)
maxSpawnTimer = 5
spawnTimer = maxSpawnTimer

--Spawn Enemies
--for i = 0, enemySpawnCount do
--	enemy = CreateEntity("Enemy")
--	spawnedEnemies[i] = enemy
--	GetTransform(enemy).position = spawnPoint
--	GetSpriteRenderer(enemy).sprite = GetSprite("Enemy")
--	GetScript(enemy).name = "Enemy"
--end

print("STARTUP DONE")
