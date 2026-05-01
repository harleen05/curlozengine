#include "window/native.hpp"
#include "config/config.hpp"
#include "core/enginestate.hpp"
#include "core/logs.hpp"

namespace clz::window
{
	std::expected<void, std::string> initializeGLFW(types::window& rWindow)
	{
		const int width = clz::config::getInt("window", "width", 800);
		const int height = clz::config::getInt("window", "height", 600);
		if (width <= 0 || height <= 0)
			return std::unexpected("Invalid window dimensions passed");

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		rWindow.Window = glfwCreateWindow(width, height, clz::config::getAppName().c_str(),
						  nullptr, nullptr);
		if (!rWindow.Window)
			return std::unexpected("Could not create window handle");

		return {};
	}

	void shutdownGLFW(types::window& rWindow)
	{
		glfwDestroyWindow(rWindow.Window);
		rWindow.Window = nullptr;
		glfwTerminate();

		clz::log::info("Window shutdown successful");
	}

	void pollEventsGLFW(const types::window& rWindow)
	{
		glfwPollEvents();

		if (glfwWindowShouldClose(rWindow.Window))
		{
			clz::state::setEngineState(clz::state::EngineState::Shutdown, "window poll events");
		}
	}
} // namespace clz::window