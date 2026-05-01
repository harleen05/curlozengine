/**
* @file scripting.hpp
 * @author curl0z
 * @brief Lua scripting subsystem.
 */

#pragma once

#include <string>

namespace clz::script
{
	/**
	 * @brief Initializes the Lua state and exposes engine bindings.
	 */
	void init();

	/**
	 * @brief Executes a Lua script file.
	 * @param path Path to the .lua file relative to working directory.
	 */
	void runScript(const std::string& path);

} // namespace clz::scripting