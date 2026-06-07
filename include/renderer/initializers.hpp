/**
 * @file initializers.hpp
 * @author curl0z
 * @brief Initializes all the vulkan context's
 */
#pragma once

#include <expected>
#include <string>

namespace clz::renderer
{
	/**
	 * @brief Initializes the device context.
	 * ie all the device handles.
	 * Can view all of them in context.hpp
	 */
	std::expected<void, std::string> initDeviceContext();

	/**
	 * @brief Initializes the swapchain context.
	 * ie all the device handles.
	 * Can view all of them in context.hpp
	 */
	std::expected<void, std::string> initSwapchainContext();

	/**
	 * @brief Initializes the pipeline context.
	 * ie all the device handles.
	 * Can view all of them in context.hpp
	 */
	std::expected<void, std::string> initPipelineContext();

	/**
	 * @brief Initializes the pipeline context.
	 * ie all the device handles.
	 * Can view all of them in context.hpp
	 */
	std::expected<void, std::string> initFrameContext();
} // namespace clz::renderer