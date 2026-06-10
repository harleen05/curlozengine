#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace clz::ecs
{
	using entity = uint32_t;

	constexpr entity NULL_ENTITY  = std::numeric_limits<uint32_t>::max();
	constexpr entity MAX_ENTITIES = 128;

	inline std::vector<entity> ecs_entities;
}