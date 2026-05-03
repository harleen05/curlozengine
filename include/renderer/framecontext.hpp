/*
 * @file framecontext.hpp
 * @author curl0z
 * @brief Contains functions for
 * initializing the pipeline context
 *
 * @note must be the last context to be initialized
 */
#pragma once

#include <expected>
#include <string>

namespace clz::renderer
{
	/*
	 * @brief Creates a command pool
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 * @note Must be the first handle to be created
	 * in this context
	 */
	std::expected<void, std::string> createCommandPool();

	/*
	 * @brief Creates all the command buffers
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 * @note Creates ${number of swapchain images}
	 * number of command buffers.
	 * Must be called after command pool has been created
	 */
	std::expected<void, std::string> createCommandBuffer();

	/*
	 * @brief Creates all the frame sync objects
	 * ie all the semaphores(render finished and image available)
	 * and fences.
	 */
	std::expected<void, std::string> createSyncObjects();

	/*
	 * @brief Destroys the command pool
	 */
	void destroyCommandPool();

	/*
	 * @brief Destroys all the frame sync objects
	 */
	void destroySyncObjects();
} // namespace clz::renderer