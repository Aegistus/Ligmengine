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

			template<typename T> SparseSet<T>& GetSparseSet()
			{
				// Get the index for T's SparseSet
				const ComponentIndex index = std::type_index(typeid(T));
				// Create the actual sparse set if needed.
				if (components[index] == nullptr)
				{
					components[index] = std::make_unique<SparseSet<T>>();
				}
				// It's safe to cast the SparseSetHolder to its subclass and return the std::unordered_map< EntityID, T > inside.
				return static_cast<SparseSet<T>&>(*components[index]);
			}

		public:
			EntityID CreateEntity();
			void DestroyEntity(EntityID entity);

			/// <summary>
			/// Checks to see if the entity has a component of type T attached. Will not add a component
			/// if it doesn't have it.
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="entity"></param>
			/// <returns></returns>
			template<typename T> bool HasComponent(EntityID entity)
			{
				SparseSet<T> components = GetSparseSet<T>();
				if (components.data.count(entity) > 0)
				{
					return true;
				}
				return false;
			}

			/// <summary>
			/// Retrieves the component attached to an entity. If the entity does not have a component
			/// of type T attached, one will be created. Use HasComponent() to check if an entity has a
			/// component if adding a component is unwanted.
			/// </summary>
			/// <typeparam name="T">The type of component.</typeparam>
			/// <param name="entity">The entity to get the component from.</param>
			/// <returns>The component found on (or added to) the entity.</returns>
			template<typename T> T& GetComponent(EntityID entity)
			{
				return GetSparseSet<T>().data[entity];
			}

			template<typename T> void AddComponent(EntityID entity)
			{
				GetSparseSet<T>().data[entity] = T {};
			}

			// Remove component from entity
			template<typename T> void RemoveComponent(EntityID entity)
			{
				GetSparseSet<T>().Drop(entity);
			}

			template<typename T> void ForEach(ComponentCallback callback)
			{
				SparseSet<T> componentSet = GetSparseSet<T>();
				for (const auto& [entity, component] : componentSet.data)
				{
					callback(entity);
				}
			}

			template<typename T, typename U> void ForEach(ComponentCallback callback)
			{
				SparseSet<T> componentSetT = GetSparseSet<T>();
				SparseSet<U> componentSetU = GetSparseSet<U>();
				for (const auto& [entity, component] : componentSetT.data)
				{
					// if the entity has the second component as well.
					if (componentSetU.data.count(entity) != 0)
					{
						callback(entity);
					}
				}
			}

			template<typename T, typename U, typename V> void ForEach(ComponentCallback callback)
			{
				SparseSet<T> componentSetT = GetSparseSet<T>();
				SparseSet<U> componentSetU = GetSparseSet<U>();
				SparseSet<V> componentSetV = GetSparseSet<V>();
				for (const auto& [entity, component] : componentSetT.data)
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