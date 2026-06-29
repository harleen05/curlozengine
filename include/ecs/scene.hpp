/**
 * @file scene.hpp
 * @author curl0z
 * @brief Scene loading — parses entity JSON and populates the ECS registry.
 *
 * @note Internal to the ECS subsystem. Use ecs.hpp instead.
 * @note config::init() must be called before loadEntities().
 */

#pragma once

#include <expected>
#include <string>

namespace clz::ecs
{
	/**
	 * @brief Loads entities from the JSON scene file into the registry.
	 *
	 * Reads the scene file path from config [entity] file key.
	 * For each entity in the JSON, creates an EnTT entity and attaches
	 * components based on what is present in the JSON.
	 *
	 * @return true on success, false on failure and prints the error
	 */
	bool loadEntities();

} // namespace clz::ecs