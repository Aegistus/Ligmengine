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
	
	template<typename T> T& ECS::GetComponent(EntityID entity)
	{
		unordered_map<EntityID, T> components = GetSparseSet<T>();
		if (components.count(entity) != 0)
		{
			return GetSparseSet<T>()[entity];
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T> void ECS::AddComponent(EntityID entity, T component)
	{
		GetSparseSet<T>().insert({ entity, component });
	}

	// Remove component from entity
	template<typename T> void ECS::RemoveComponent(EntityID entity)
	{
		GetSparseSet<T>().Drop(entity);
	}

	template<typename T> void ECS::ForEach(ComponentCallback callback)
	{
		unordered_map<EntityID, T> componentSet = GetSparseSet<T>();
		for (auto entity = componentSet.begin(); entity != componentSet.end(); entity++)
		{
			callback(entity);
		}
	}

	template<typename T, typename U> void ECS::ForEach(ComponentCallback callback)
	{
		unordered_map<EntityID, T> componentSetT = GetSparseSet<T>();
		unordered_map<EntityID, U> componentSetU = GetSparseSet<U>();
		for (auto entity = componentSetT.begin(); entity != componentSetT.end(); entity++)
		{
			// if the entity has the second component as well.
			if (componentSetU.count(entity) != 0)
			{
				callback(entity);
			}
		}
	}

	template<typename T, typename U, typename V> void ECS::ForEach(ComponentCallback callback)
	{
		unordered_map<EntityID, T> componentSetT = GetSparseSet<T>();
		unordered_map<EntityID, U> componentSetU = GetSparseSet<U>();
		unordered_map<EntityID, V> componentSetV = GetSparseSet<V>();
		for (auto entity = componentSetT.begin(); entity != componentSetT.end(); entity++)
		{
			// if the entity has the second and third component as well.
			if (componentSetU.count(entity) != 0 && componentSetV.count(entity))
			{
				callback(entity);
			}
		}
	}
}