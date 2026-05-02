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