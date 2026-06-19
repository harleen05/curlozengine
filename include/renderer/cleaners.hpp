/**
 * @file cleaners.hpp
 * @brief Cleanup utilities for renderer contexts.
 *
 * @note These functions should be called during renderer shutdown.
 * and must be called before destroying the window
 */
#pragma once

namespace clz::renderer
{
	/**
	 * @brief Destroys the device context.
	 * @note Must destroy instance first,
	 * then surface, then the device
	 */
	void destroyDeviceContext();

	/**
	 * @brief Destroys the swapchain context.
	 */
	void destroySwapchainContext();

	/**
	 * @brief Destroys the pipeline context.
	 */
	void destroyPipelineContext();

	/**
	 * @brief Destroys the frame context.
	 */
	void destroyFrameContext();
} // namespace clz::renderer
