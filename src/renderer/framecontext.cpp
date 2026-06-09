/*
 * @file framecontext.cpp
 * @author curl0z
 * @brief Implementation of the initialization
 * and cleanup of frame context
 */
#include "renderer/framecontext.hpp"
#include "core/logs.hpp"
#include "renderer/variables.hpp"
#include <vulkan/vulkan.h>

namespace clz::renderer
{
	std::expected<void, std::string> createCommandPool()
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = r_deviceContext.graphicsFamily.value();
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(r_deviceContext.device, &commandPoolInfo, nullptr,
					&r_frameContext.commandPool) != VK_SUCCESS)
		{
			clz::log::error("could not create command pool :(");
			return std::unexpected("could not create command pool :(");
		}

		clz::log::debug("created command pool");
		return {};
	}

	std::expected<void, std::string> createCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = r_frameContext.commandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = r_swapchainContext.images.size();

		r_frameContext.commandBuffer.resize(
		    static_cast<size_t>(r_swapchainContext.images.size()));

		if (vkAllocateCommandBuffers(r_deviceContext.device, &allocateInfo,
					     r_frameContext.commandBuffer.data()) != VK_SUCCESS)
		{
			clz::log::error("renderer: Failed to allocate command buffers");
			return std::unexpected("renderer: failed to create command buffers");
		}

		clz::log::debug("created command buffers");

		return {};
	}

	std::expected<void, std::string> createSyncObjects()
	{

		r_frameContext.imageAvailableSemaphores.resize(
		    static_cast<size_t>(FRAMES_IN_FLIGHT));
		r_frameContext.inFlightFences.resize(static_cast<size_t>(FRAMES_IN_FLIGHT));
		for (size_t i = 0; i < FRAMES_IN_FLIGHT; ++i)
		{
			VkSemaphoreCreateInfo semaphoreInfo{
			    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			};
			if (vkCreateSemaphore(r_deviceContext.device, &semaphoreInfo, nullptr,
					      &r_frameContext.imageAvailableSemaphores[i]) !=
			    VK_SUCCESS)
			{
				clz::log::error("renderer: Could not create semaphores");
				return std::unexpected("renderer: Could not create semaphores");
			}

			VkFenceCreateInfo fenceInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
						    .flags = VK_FENCE_CREATE_SIGNALED_BIT};
			if (vkCreateFence(r_deviceContext.device, &fenceInfo, nullptr,
					  &r_frameContext.inFlightFences[i]) != VK_SUCCESS)
			{
				clz::log::error("renderer: Could not create fence");
				return std::unexpected("renderer: Could not create fence");
			}
		}

		r_frameContext.renderFinishedSemaphores.resize(
		    static_cast<size_t>(r_swapchainContext.images.size()));
		for (size_t i = 0; i < r_swapchainContext.images.size(); ++i)
		{
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			if (vkCreateSemaphore(r_deviceContext.device, &semaphoreInfo, nullptr,
					      &r_frameContext.renderFinishedSemaphores[i]) !=
			    VK_SUCCESS)
			{
				clz::log::error("renderer: Could not create semaphores");
				return std::unexpected("renderer: Could not create semaphores");
			}
		}

		clz::log::debug("created All semaphores and fences");
		return {};
	}

	void destroyCommandPool()
	{
		vkDestroyCommandPool(r_deviceContext.device, r_frameContext.commandPool, nullptr);
		clz::log::debug("destroyed command pool");
	}

	void destroySyncObjects()
	{
		for (const auto imageAvailableSemaphore : r_frameContext.imageAvailableSemaphores)
		{
			vkDestroySemaphore(r_deviceContext.device, imageAvailableSemaphore,
					   nullptr);
		}

		for (const auto inFlightFence : r_frameContext.inFlightFences)
		{
			vkDestroyFence(r_deviceContext.device, inFlightFence, nullptr);
		}

		for (const auto renderFinishedSemaphore : r_frameContext.renderFinishedSemaphores)
		{
			vkDestroySemaphore(r_deviceContext.device, renderFinishedSemaphore,
					   nullptr);
		}

		clz::log::debug("destroyed semaphores and fences");
	}

} // namespace clz::renderer