/*
 * @file vulkanhelper.hpp
 * @brief Public header to help vulkan
 */
#pragma once

#include <expected>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace clz::window
{
	std::expected<void, std::string>
	getRequiredVulkanExtensions(std::vector<const char*>& rRequiredExtensions);
	std::expected<void, std::string> createVulkanSurface(VkInstance instance,
							     VkSurfaceKHR& rSurface);
} // namespace clz::window