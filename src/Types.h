#pragma once

#include <string>
#include "glm/glm.hpp"
#include <typeindex>

namespace Ligmengine
{
	typedef double real;
	typedef std::string string;
	typedef glm::vec2 vector2;
	typedef glm::vec3 vector3;
	typedef glm::vec4 vector4;
	typedef glm::mat4 matrix4x4;
	// ECS
	typedef long EntityID;
	typedef std::type_index ComponentIndex;
	class Engine;
}