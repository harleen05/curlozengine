/**
 * @file scene.cpp
 * @author curl0z
 * @brief Scene loading implementation.
 *
 * Parses the entity JSON file and populates the EnTT registry
 * with entities and their components.
 */

#include "ecs/scene.hpp"
#include "config/config.hpp"
#include "core/logs.hpp"
#include "ecs/components.hpp"
#include "ecs/entitymanager.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

namespace clz::ecs
{
	std::expected<void, std::string> loadEntities()
	{
		clz::log::debug("Loading entities");

		// Read scene file path from config
		const auto entityFile = clz::config::getString("entity", "file", "");
		if (std::string(entityFile).empty())
			return std::unexpected("No entity file specified in config [entity] file");

		// Parse JSON
		std::ifstream file(entityFile);
		if (!file.is_open())
			return std::unexpected("Could not open entity file: " +
					       std::string(entityFile));

		auto json = nlohmann::json::parse(file);

		// Create entities from JSON
		for (auto& entityData : json["entities"])
		{
			auto entity = createEntity();

			// Attach TransformComponent if present
			if (entityData["components"].contains("transform"))
			{
				auto& t = entityData["components"]["transform"];
				TransformComponent transformComponent(
					clz::math::vec3(t["position"][0], t["position"][1], t["position"][2]),
					clz::math::vec3(t["rotation"][0], t["rotation"][1], t["rotation"][2]),
					clz::math::vec3(t["scale"][0], t["scale"][1], t["scale"][2]));

				addComponent<TransformComponent>(entity, transformComponent);
			}
		}

		// Log loaded components
		auto& componentArray = getComponentArray<TransformComponent>();
		for (const auto component : componentArray)
		{
			const auto pos = component.position;
			clz::log::debug("Entity loaded at position: " + std::to_string(pos.x) +
					", " + std::to_string(pos.y) + ", " +
					std::to_string(pos.z));
		}

		return {};
	}

} // namespace clz::ecs
