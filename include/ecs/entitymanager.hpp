/**
 * @file entitymanager.hpp
 * @author curl0z
 * @brief Entity creation and destruction functions.
 *
 * Entities are just integer IDs. All data lives in component
 * storages in componentmanager.hpp.
 */

#pragma once

#include "componentmanager.hpp"
#include "entity.hpp"
#include <cstdint>
#include <vector>

namespace clz::ecs
{
	/**
	 * @brief Creates a new entity and returns its ID.
	 *
	 * Entity ID is the current size of ecs_entities
	 *
	 * @return Unique entity ID.
	 */
	inline uint32_t createEntity()
	{
		uint32_t id = ecs_entities.size();
		ecs_entities.emplace_back(id);
		return id;
	}



	/**
	 * @brief Removes an entity and all its components.
	 *
	 * Strips all components via removeAllComponentsForEntity.
	 * The entity ID slot in ecs_entities is not recycled tho.
	 * Maybe added later.
	 *
	 * @param e Entity to remove.
	 */
	inline void removeEntity(const entity e)
	{
		removeAllComponentsForEntity(e);
	}

	/**
	 * @brief Clears the entity list without destroying components.
	 *
	 * @note Call deleteAllComponents() before or after this to
	 * avoid dangling component data.
	 */
	inline void clearEntities()
	{
		ecs_entities.clear();
	}

} // namespace clz::ecs