/**
 * @file native.hpp
 * @author curl0z
 * @brief Low level GLFW initialization and teardown functions.
 *
 * @note These are internal to the window subsystem — do not call directly.
 * Use the public window.hpp interface instead.
 */

#pragma once

#include "GLFW/glfw3.h"
#include <expected>
#include <string>

namespace clz::window
{
	/**
	 * @brief Initializes GLFW and creates the window.
	 *
	 * @note Reads window dimensions from the config subsystem.
	 *
	 * @param pWindow pointer to the GLFW window
	 * @return void on success, error string on failure.
	 */
	std::expected<void, std::string> initializeGLFW(GLFWwindow** pWindow);

	/**
	 * @brief Destroys the GLFW window and terminates GLFW.
	 * @param pWindow pointer to the window handle to destroy.
	 */
	void shutdownGLFW(GLFWwindow** pWindow);

	/**
	 * @brief Polls GLFW events and checks for window close or escape key.
	 * @param pWindow pointer to the active window.
	 */
	void pollEventsGLFW(GLFWwindow** pWindow);

} // namespace clz::window