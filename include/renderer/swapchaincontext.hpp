/**
 * @file swapchaincontext.hpp
 * author curl0z
 * @brief Contains functions for initializing and
 * destroying all the handles in swapchain context.
 */
#pragma once

#include <expected>
#include <string>


namespace clz::renderer
{
	/**
	 * @brief Creates swapchain, initializes all the
	 * images and image views
	 * and sets all the other swapchain settings
	 */
	std::expected<void, std::string> createSwapchain();

	/**
	 * @brief Recreates the swapchain.
	 *
	 * Called when the swapchain becomes invalid
	 * Destroys and rebuilds the whole swapchain context
	 *
	 * @note window subsystem also calls it internally
	 * whenever window is resized
	 */
	void recreateSwapchainContext();

	/**
	 * @brief Destroys the all the handles in
	 * the swapchain context
	 */
	void destroySwapchain();
} // namespace clz::renderer
