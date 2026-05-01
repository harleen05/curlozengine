/**
 * @file window_types.hpp
 * @author curl0z
 * @brief Core window data types for the window subsystem.
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace clz::window::types
{
	/**
	 * @brief Holds the native GLFW window handle.
	 */
	struct window
	{
		GLFWwindow* Window; ///< Raw GLFW window pointer.
	};

} // namespace clz::window::types