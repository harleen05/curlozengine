/**
 * @file ecs.hpp
 * @author curl0z
 * @brief Public interface for the ECS subsystem.
 *
 * Manages entity lifecycle and component systems.
 * Loads entities from a JSON scene file on init.
 *
 * @note config::init() must be called before ecs::init() as
 * the scene file path is read from config.
 */

#pragma once

namespace clz::ecs
{
	/**
	 * @brief Initializes the ECS subsystem and loads entities from the scene file.
	 *
	 * Clears the registry, then calls loadEntities() to populate it
	 * from the JSON file specified in config [entity] file.
	 */
	void init();

	/**
	 * @brief Runs all ECS systems for one frame.
	 */
	void update();

	/**
	 * @brief Clears the registry and destroys all entities and components.
	 */
	void shutdown();

} // namespace clz::ecs