/**
 * @file luainterface.hpp
 * @author curl0z
 * @brief Defines all the interfaces such as functions and keyboard binds etc. to lua
 */

#pragma once

#include <sol/sol.hpp>

namespace clz::script
{
	/*
	 * @brief Bind all keyboard keys to lua
	 * @param rLua Reference to the active lua interface
	 */
	void bindKeyboardKeys(sol::state& rLua);

	/*
	 * @brief Provides logging capability to lua. Internally calls clz::log::info
	 * @param msg reference to message called from any Lua file
	 */
	void log(const std::string& msg);

	/*
	 * @brief Ask if a keyboard key is pressed or not
	 * @param key Received from lua as an integer Matching GLFW key codes
	 */
	bool isKeyPressed(int key);

	/*
	 * @brief Ask if a keyboard key is released or not
	 * @param key Received from lua as an integer Matching GLFW key codes
	 */
	bool isKeyReleased(int key);
} // namespace clz::script