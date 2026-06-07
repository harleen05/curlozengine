/**
 * @file ecs.cpp
 * @author curl0z
 * @brief ECS subsystem implementation.
 */

#include "ecs/ecs.hpp"
#include "core/logs.hpp"
#include "ecs/scene.hpp"
#include "ecs/variables.hpp"

namespace clz::ecs
{
	void init()
	{
		// Clear any existing state before loading
		ecs_registry.clear();

		auto result = loadEntities();
		if (!result)
		{
			clz::log::error(result.error());
			return;
		}

		clz::log::info("ECS subsystem initialized");
	}

	void update()
	{
		// Systems run here — iterate views and process components
	}

	void shutdown()
	{
		ecs_registry.clear();
		clz::log::info("ECS subsystem shutdown");
	}

} // namespace clz::ecs