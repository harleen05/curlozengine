#pragma once

#include "componentmanager.hpp"
#include <vector>
#include <cstdint>
#include "entity.hpp"



namespace clz::ecs
{
	inline uint32_t createEntity()
	{
		uint32_t id = ecs_entities.size();
		ecs_entities.emplace_back(id);
		return id;
	}

	inline void removeEntity(const entity e)
	{
		removeAllComponentsForEntity(e);
	}

	inline void clearEntities()
	{
		ecs_entities.clear();
	}
}