#pragma once

#include <string>
#include "math/math.hpp"

namespace clz::ecs
{
	struct TransformComponent
	{
		clz::math::vec3 position = {0, 0, 0};
		clz::math::vec3 rotation = {0, 0, 0};
		clz::math::vec3 scale    = {1, 1, 1};
	};

	struct MeshComponent
	{
		uint32_t meshId = 0;
	};

	struct RigidBodyComponent
	{
		uint32_t bodyId = 0;
	};

	struct AudioSourceComponent
	{
		uint32_t sourceId = 0;
	};

	struct ScriptComponent
	{
		std::string scriptPath;
	};
}