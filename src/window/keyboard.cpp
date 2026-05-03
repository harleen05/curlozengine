/*
 * @file keyboard.cpp
 * @author curl0z
 * @brief implementation of keyboard events of the window
 */
#include "window/keyboard.hpp"
#include "window/window.hpp"

namespace clz::window
{
	bool isPressed(Key key)
	{
		return glfwGetKey(getWindowHandle(), static_cast<int>(key)) == GLFW_PRESS;
	}

	bool isReleased(Key key)
	{
		return glfwGetKey(getWindowHandle(), static_cast<int>(key)) == GLFW_RELEASE;
	}
}