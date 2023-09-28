#pragma once

#include <Types.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <Components.h>
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
					components[index] = std::make_unique<SparseSet>();
				}
				// It's safe to cast the SparseSetHolder to its subclass and return the std::unordered_map< EntityID, T > inside.
				return (*components[index]).data;
			}

		public:
			EntityID CreateEntity();
			void DestroyEntity(EntityID entity);
			template<typename T> T& GetComponent(EntityID entity);
			template<typename T> void AddComponent(EntityID entity, T& component);
			template<typename T> void RemoveComponent(EntityID entity);
			template<typename T> void ForEach(ComponentCallback callback);
			template<typename T, typename U> void ForEach(ComponentCallback callback);
			template<typename T, typename U, typename V> void ForEach(ComponentCallback callback);
	};

}