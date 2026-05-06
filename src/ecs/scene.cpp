#include "ecs/scene.hpp"
#include "config/config.hpp"
#include "core/logs.hpp"
#include <nlohmann/json.hpp>

#include "ecs/components.hpp"
#include "ecs/variables.hpp"
#include <fstream>

#include <fmt/format.h>

namespace clz::ecs
{
	std::expected<void, std::string> loadEntities()
	{
		clz::log::info("Loading entities");
		const auto entityFile = clz::config::getString("entity", "file", "Lmao XD");
		auto json = nlohmann::json::parse(std::ifstream(entityFile));

		for (auto& entityData : json["entities"])
		{
			auto entity = ecs_registry.create();

			if (entityData["components"].contains("transform"))
			{
				auto& t = entityData["components"]["transform"];
				ecs_registry.emplace<TransformComponent>(entity,
					clz::math::vec3(t["position"][0], t["position"][1], t["position"][2]),
					clz::math::vec3(t["rotation"][0], t["rotation"][1], t["rotation"][2]),
					clz::math::vec3(t["scale"][0], t["scale"][1], t["scale"][2])
				);
			}
		}
		// all entities with both Transform
		auto view = ecs_registry.view<TransformComponent>();
		for (const auto entity : view)
		{
			const auto pos = view.get<TransformComponent>(entity).position;
			clz::log::info("created entity with position: " + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z));
		}
		size_t count = 0;
		for (auto entity : ecs_registry.view<TransformComponent>())
			count++;
		clz::log::info("entity count: " + std::to_string(count));

		return {};
	}
}