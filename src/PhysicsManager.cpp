#include "PhysicsManager.h"
#include "Engine.h"
#include "Components.h"

namespace Ligmengine
{
	void PhysicsManager::Startup()
	{

	}

	void PhysicsManager::Update()
	{
		gEngine.ecs.ForEach<RigidBody, Transform>
		(
			[&](EntityID e)
			{
				RigidBody& rb = gEngine.ecs.GetComponent<RigidBody>(e);
				Transform& t = gEngine.ecs.GetComponent<Transform>(e);
				t.position += vector3(rb.velocity.x, rb.velocity.y, 0);
				if (rb.useGravity)
				{
					rb.velocity += gravity * gEngine.deltaTime;
				}
			}
		);
	}

	void PhysicsManager::Shutdown()
	{

	}
}
