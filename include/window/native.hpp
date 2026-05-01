/**
 * @file native.hpp
 * @author curl0z
 * @brief Low level GLFW initialization and teardown functions.
 *
 * @note These are internal to the window subsystem — do not call directly.
 * Use the public window.hpp interface instead.
 */

#pragma once

#include "window_types.hpp"
#include <expected>
#include <string>

namespace clz::window
{
	/**
	 * @brief Initializes GLFW and creates the window.
	 *
	 * @note Reads window dimensions from the config subsystem.
	 *
	 * @param rWindow Reference to the window struct to populate.
	 * @return void on success, error string on failure.
	 */
	std::expected<void, std::string> initializeGLFW(types::window& rWindow);

	/**
	 * @brief Destroys the GLFW window and terminates GLFW.
	 * @param rWindow Reference to the window struct to destroy.
	 */
	void shutdownGLFW(types::window& rWindow);

	/**
	 * @brief Polls GLFW events and checks for window close or escape key.
	 * @param rWindow Reference to the active window.
	 */
	void pollEventsGLFW(const types::window& rWindow);

} // namespace clz::window