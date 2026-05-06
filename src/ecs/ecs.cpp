#include "ecs/ecs.hpp"
#include "ecs/scene.hpp"
#include "core/logs.hpp"
#include "ecs/variables.hpp"

namespace clz::ecs
{
	void init()
	{
		ecs_registry.clear();
		auto loadedEntities = loadEntities();
		if (!loadedEntities)
		{
			clz::log::error(loadedEntities.error());
		}
		clz::log::info("Initialized entity system");
	}

	void shutdown()
	{
		ecs_registry.clear();
		clz::log::info("Shut down entity system");
	}
}