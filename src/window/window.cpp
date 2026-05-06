/*
 * @file window.cpp
 * @author curl0z
 * @brief Implementation of the window public header
 */
#define GLFW_INCLUDE_VULKAN
#include "window/window.hpp"
#include "core/logs.hpp"
#include "window/native.hpp"

namespace clz::window
{
	/// @brief Internal window state.
	static GLFWwindow* w_window = nullptr;
} // namespace clz::window

namespace clz::window
{
	void init()
	{
		auto result = initializeGLFW(&w_window);
		if (!result) [[unlikely]]
		{
			clz::log::error(result.error());
			return;
		}
		clz::log::info("Initialized window system");
	}

	void shutdown()
	{
		shutdownGLFW(&w_window);
	}

	void update()
	{
		pollEventsGLFW(&w_window);
	}

	GLFWwindow *getWindowHandle()
	{
		return w_window;
	}

	void getFramebufferExtents(int *width, int *height)
	{
		glfwGetFramebufferSize(w_window, width, height);
	}
} // namespace clz::window