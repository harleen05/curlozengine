/**
 * @file swapchaincontext.cpp
 * @author curl0z
 * @brief Implementation of the initialization
 * and cleanup of swapchain context
 */
#include "renderer/swapchaincontext.hpp"
#include "config/config.hpp"
#include "core/logs.hpp"
#include "renderer/cleaners.hpp"
#include "renderer/initializers.hpp"
#include "renderer/variables.hpp"
#include "window/window.hpp"
#include <string>

namespace clz::renderer
{
	std::expected<void, std::string> createSwapchain()
	{
		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(r_deviceContext.gpu,
							  r_deviceContext.surface, &capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(r_deviceContext.gpu, r_deviceContext.surface,
						     &formatCount, nullptr);
		if (formatCount == 0)
		{
			clz::log::error("Uhh.... Selected gpu does not have any format lol");
			return std::unexpected("Uhh.... Selected gpu does not have any format lol");
		}
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(r_deviceContext.gpu, r_deviceContext.surface,
						     &formatCount, formats.data());

		// 1.2 get presentation modes;
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(
		    r_deviceContext.gpu, r_deviceContext.surface, &presentModeCount, nullptr);
		if (presentModeCount == 0)
		{
			clz::log::error("renderer/initializers.cpp: Selected physical device does "
					"not have a valid presentation mode");
			return std::unexpected("renderer/initializers.cpp: Selected physical "
					       "device does not have a valid presentation mode");
		}
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(r_deviceContext.gpu,
							  r_deviceContext.surface,
							  &presentModeCount, presentModes.data());

		// Choosing the right things for our swapchain

		r_swapchainContext.format = formats[0];
		for (const auto& availableFormat : formats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				r_swapchainContext.format = availableFormat;
				break;
			}
		}

		// Primarily select mailbox
		r_swapchainContext.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (clz::config::getBool("window", "triplebuffering", false))
		{

			for (const auto& availablePresentMode : presentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					r_swapchainContext.presentMode =
					    VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
			}
		}

		// Extent
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			r_swapchainContext.extent = capabilities.currentExtent;
		}
		else
		{
			int width, height;
			clz::window::getFramebufferExtents(&width, &height);

			VkExtent2D actualExtent = {static_cast<uint32_t>(width),
						   static_cast<uint32_t>(height)};

			r_swapchainContext.extent.width =
			    std::clamp(actualExtent.width, capabilities.minImageExtent.width,
				       capabilities.maxImageExtent.width);
			r_swapchainContext.extent.height =
			    std::clamp(actualExtent.height, capabilities.minImageExtent.height,
				       capabilities.maxImageExtent.height);
		}

		// Finally creating the swapchain
		uint32_t imageCount = capabilities.minImageCount + 1;
		if (imageCount > capabilities.maxImageCount && capabilities.maxImageCount > 0)
			imageCount = capabilities.maxImageCount;
		clz::log::debug("max image count: " + std::to_string(imageCount));

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = r_deviceContext.surface;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = r_swapchainContext.format.format;
		swapchainInfo.imageColorSpace = r_swapchainContext.format.colorSpace;
		swapchainInfo.imageExtent = r_swapchainContext.extent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		uint32_t QueueFamilyIndices[] = {r_deviceContext.graphicsFamily.value(),
						 r_deviceContext.presentFamily.value()};
		if (r_deviceContext.graphicsFamily != r_deviceContext.presentFamily)
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = 2;
			swapchainInfo.pQueueFamilyIndices = QueueFamilyIndices;
		}
		else
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0;
			swapchainInfo.pQueueFamilyIndices = nullptr;
		}
		swapchainInfo.preTransform = capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = r_swapchainContext.presentMode;
		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(r_deviceContext.device, &swapchainInfo, nullptr,
					 &r_swapchainContext.swapchain) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("could not create swapchain sadly :(");
			return std::unexpected("could not create swapchain");
		}

		vkGetSwapchainImagesKHR(r_deviceContext.device, r_swapchainContext.swapchain,
					&imageCount, nullptr);
		r_swapchainContext.images.resize(imageCount);
		vkGetSwapchainImagesKHR(r_deviceContext.device, r_swapchainContext.swapchain,
					&imageCount, r_swapchainContext.images.data());

		// retrieving image views
		r_swapchainContext.imageViews.resize(r_swapchainContext.images.size());
		for (size_t i = 0; i < r_swapchainContext.images.size(); ++i)
		{
			VkImageViewCreateInfo imageViewInfo = {};
			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.image = r_swapchainContext.images[i];
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewInfo.format = r_swapchainContext.format.format;
			imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewInfo.subresourceRange.baseMipLevel = 0;
			imageViewInfo.subresourceRange.levelCount = 1;
			imageViewInfo.subresourceRange.baseArrayLayer = 0;
			imageViewInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(r_deviceContext.device, &imageViewInfo, nullptr,
					      &r_swapchainContext.imageViews[i]) != VK_SUCCESS)
			{
				clz::log::error(
				    "renderer/initializers.cpp: Could not create image views");
				return std::unexpected("could not create image views");
			}
		}

		clz::log::info("renderer: created swapchain");
		return {};
	}

	void recreateSwapchainContext()
	{
		vkDeviceWaitIdle(r_deviceContext.device);

		destroySwapchainContext();
		if (const auto result = initSwapchainContext(); !result) [[unlikely]]
		{
			clz::log::error("Mid loop, failed to recreate swapchain");
		}
	}

	void destroySwapchain()
	{
		for (const auto imageView : r_swapchainContext.imageViews)
		{
			vkDestroyImageView(r_deviceContext.device, imageView, nullptr);
		}
		vkDestroySwapchainKHR(r_deviceContext.device, r_swapchainContext.swapchain,
				      nullptr);

		clz::log::info("renderer: destroyed swapchain context");
	}
} // namespace clz::renderer
