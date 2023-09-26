#include <ECS.h>
#include <vector>
#include <Types.h>

using namespace std;

namespace Ligmengine
{
	EntityID ECS::CreateEntity(string name)
	{
		return 0;
	}

	EntityID ECS::CreateEntity(string name, vector<Component> components)
	{
		return 0;
	}

	void ECS::DestroyEntity(EntityID entity)
	{

	}

	template<typename T> T& ECS::GetComponent(EntityID entity)
	{

	}

	template<typename T> void ECS::RemoveComponent(EntityID entity)
	{

	}

	template<typename T> void ECS::ForEach(ComponentCallback)
	{

	}
}