/*
 * @file vulkanhelper.cpp
 * @author curl0z
 * @brief Implementation of all the vulkan helper functions
 */
#include "window/vulkanhelper.hpp"
#include <GLFW/glfw3.h>
#include "window/window.hpp"

namespace clz::window
{
	std::expected<void, std::string> getRequiredVulkanExtensions(std::vector<const char*>& rRequiredExtensions)
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		if (glfwExtensionCount == 0 || glfwExtensions == nullptr)
		{
			return std::unexpected("Could not retrieve extensions required for instance by window");
		}
		rRequiredExtensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return {};
	}
	std::expected<void, std::string> createVulkanSurface(const VkInstance instance, VkSurfaceKHR& rSurface)
	{
		if (glfwCreateWindowSurface(instance, getWindowHandle(), nullptr, &rSurface) != VK_SUCCESS)
			return std::unexpected("Could not create window surface");

		return {};
	}
}