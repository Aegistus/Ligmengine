#pragma once
#include <Types.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <SparseSet.h>

using namespace std;

namespace Ligmengine
{
	typedef std::function<void(EntityID)> ComponentCallback;

	class ECS
	{
		private:
			EntityID nextID = 0;
			unordered_map<ComponentIndex, std::unique_ptr<SparseSetHolder>> components;

			template<typename T> std::unordered_map<EntityID, T> GetSparseSet()
			{
				// Get the index for T's SparseSet
				const ComponentIndex index = std::type_index(typeid(T));
				// Create the actual sparse set if needed.
				if (components[index] == nullptr)
				{
					components[index] = std::make_unique<SparseSet<T>>();
				}
				// It's safe to cast the SparseSetHolder to its subclass and return the std::unordered_map< EntityID, T > inside.
				return static_cast<SparseSet<T>&>(*components[index]).data;
			}

		public:
			EntityID CreateEntity();
			void DestroyEntity(EntityID entity);

			template<typename T> T& GetComponent(EntityID entity)
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

			template<typename T> void AddComponent(EntityID entity, T component)
			{
				GetSparseSet<T>().insert({ entity, component });
			}

			// Remove component from entity
			template<typename T> void RemoveComponent(EntityID entity)
			{
				GetSparseSet<T>().Drop(entity);
			}

			template<typename T> void ForEach(ComponentCallback callback)
			{
				unordered_map<EntityID, T> componentSet = GetSparseSet<T>();
				for (const auto& [entity, component] : componentSet)
				{
					callback(entity);
				}
			}

			template<typename T, typename U> void ForEach(ComponentCallback callback)
			{
				unordered_map<EntityID, T> componentSetT = GetSparseSet<T>();
				unordered_map<EntityID, U> componentSetU = GetSparseSet<U>();
				for (const auto& [entity, component] : componentSetT)
				{
					// if the entity has the second component as well.
					if (componentSetU.count(entity) != 0)
					{
						callback(entity);
					}
				}
			}

			template<typename T, typename U, typename V> void ForEach(ComponentCallback callback)
			{
				unordered_map<EntityID, T> componentSetT = GetSparseSet<T>();
				unordered_map<EntityID, U> componentSetU = GetSparseSet<U>();
				unordered_map<EntityID, V> componentSetV = GetSparseSet<V>();
				for (const auto& [entity, component] : componentSetT)
				{
					// if the entity has the second and third component as well.
					if (componentSetU.count(entity) != 0 && componentSetV.count(entity))
					{
						callback(entity);
					}
				}
			}
	};

}