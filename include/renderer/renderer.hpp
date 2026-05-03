/*
 * @file renderer.hpp
 * @author curl0z
 * @brief Public interface for renderer subsystem
 *
 * Wraps all the vulkan functions into subsystem functions
 *
 * @note init function must be called after window and config has been initialized
 */

#pragma once

namespace clz::renderer
{
	/*
	 * @brief Initializes all the vulkan handles divided into context's
	 * @note logs an error and returns on early failure
	 */
	void init();

	/*
	 * @brief updates the renderer
	 * @note logs an error if swapchain is outdated or some rare event happens
	 */
	void update(float deltaTime);

	/*
	 * @brief Destroys all the vulkan context's
	 */
	void shutdown();
} // namespace clz::renderer
