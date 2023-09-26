#pragma once
#include <Types.h>
#include <functional>
#include <vector>
#include <Component.h>

using namespace std;

namespace Ligmengine
{
	typedef std::function<void()> ComponentCallback;

	class ECS
	{
		public:
			EntityID CreateEntity(string name);
			EntityID CreateEntity(string name, vector<Component> components);
			void DestroyEntity(EntityID entity);
			template<typename T> T& GetComponent(EntityID entity);
			template<typename T> void RemoveComponent(EntityID entity);
			template<typename T> void ForEach(ComponentCallback);
	};
}