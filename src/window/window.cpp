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
} // namespace clz::window