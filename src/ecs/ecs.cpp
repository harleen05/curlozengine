/**
 * @file ecs.cpp
 * @author curl0z
 * @brief ECS subsystem implementation.
 */

#include "ecs/ecs.hpp"
#include "core/logs.hpp"
#include "ecs/entitymanager.hpp"
#include "ecs/scene.hpp"

namespace clz::ecs
{
	bool init()
	{
		if (!loadEntities())
		{
			clz::log::error("Failed to load entities");
			return false;
		}

		clz::log::info("ECS subsystem initialized");
		return true;
	}

	void update()
	{
		// Systems run here — iterate views and process components
	}

	void shutdown()
	{
		clearEntities();
		clz::log::info("ECS subsystem shutdown");
	}

} // namespace clz::ecs