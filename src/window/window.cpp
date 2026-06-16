/**
 * @file window.cpp
 * @author curl0z
 * @brief Implementation of the window public header
 */
#include "window/window.hpp"
#include "core/logs.hpp"
#include "window/native.hpp"
#include "window/variables.hpp"
#include "window/vulkanhelper.hpp"

namespace clz::window
{
	void init()
	{
		// Initialize window
		if (auto result = initializeGLFW(&w_window); !result)
		{
			clz::log::error(result.error());
			return;
		}

		// Initialize all callback functions
		glfwSetFramebufferSizeCallback(w_window, hintRendererAboutResize);

		clz::log::debug("Initialized window system");
	}

	void shutdown()
	{
		shutdownGLFW(&w_window);
	}

	void update()
	{
		pollEventsGLFW(&w_window);
	}

	void getFramebufferExtents(int* width, int* height)
	{
		glfwGetFramebufferSize(w_window, width, height);
	}
} // namespace clz::window
