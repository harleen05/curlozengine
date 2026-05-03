#pragma once

#include <expected>
#include <vector>
#include <string>
#include <vulkan/vulkan.h>

namespace clz::window
{
	std::expected<void, std::string> getRequiredVulkanExtensions(std::vector<const char*>& rRequiredExtensions);
	std::expected<void, std::string> createVulkanSurface(VkInstance instance, VkSurfaceKHR& rSurface);
}