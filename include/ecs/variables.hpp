/**
* @file variables.hpp
 * @author curl0z
 * @brief Global ECS registry instance.
 *
 * @note Include this in any ECS .cpp that needs direct registry access.
 * Do not include in public facing headers.
 */

#pragma once

#include <entt/entt.hpp>

namespace clz::ecs
{
	/// @brief Global EnTT registry. Owns all entities and components.
	inline entt::registry ecs_registry;

} // namespace clz::ecs