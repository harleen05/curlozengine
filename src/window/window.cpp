#define GLFW_INCLUDE_VULKAN
#include "window/window.hpp"
#include "core/logs.hpp"
#include "window/native.hpp"
#include "window/window_types.hpp"

namespace clz::window
{
	/// @brief Internal window state.
	types::window w_window;
} // namespace clz::window

namespace clz::window
{
	void init()
	{
		auto result = initializeGLFW(w_window);
		if (!result) [[unlikely]]
		{
			clz::log::error(result.error());
			return;
		}
	}

	void shutdown()
	{
		shutdownGLFW(w_window);
	}

	void update()
	{
		pollEventsGLFW(w_window);
	}
} // namespace clz::window