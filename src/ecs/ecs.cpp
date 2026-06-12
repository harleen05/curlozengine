/**
 * @file ecs.cpp
 * @author curl0z
 * @brief ECS subsystem implementation.
 */

#include "ecs/ecs.hpp"
#include "core/logs.hpp"
#include "ecs/componentmanager.hpp"
#include "ecs/entitymanager.hpp"
#include "ecs/scene.hpp"

namespace clz::ecs
{
	void init()
	{
		if (auto result = loadEntities(); !result)
		{
			clz::log::error(result.error());
			return;
		}

		clz::log::debug("ECS subsystem initialized");
	}

	void update()
	{
		// Systems run here — iterate views and process components
	}

	void shutdown()
	{
		deleteAllComponents();
		clearEntities();
		clz::log::debug("ECS subsystem shutdown");
	}

} // namespace clz::ecs