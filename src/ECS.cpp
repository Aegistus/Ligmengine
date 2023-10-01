#include <ECS.h>
#include <vector>
#include <Types.h>

using namespace std;

namespace Ligmengine
{
	EntityID ECS::CreateEntity()
	{
		EntityID id = nextID;
		nextID++;
		return id;
	}

	// Destroy entity (removes all components
	void ECS::DestroyEntity(EntityID entity)
	{
		for (const auto& [index, componentSet] : components)
		{
			componentSet->Drop(entity);
		}
	}
}