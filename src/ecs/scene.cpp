#include "ecs/scene.hpp"
#include <nlohmann/json.hpp>
#include "config/config.hpp"

namespace clz::ecs
{
	std::expected<void, std::string> loadScene()
	{
		auto json = nlohmann::json::parse(clz::config::getString("entity", "file", "Lmao XD"));


		return {};
	}
}