/**
 * @file mainloop.cpp
 * @author curl0z
 * @brief Implementation of the main loop of the frame
 */
#include "renderer/mainloop.hpp"
#include "core/logs.hpp"
#include "renderer/image.hpp"
#include "renderer/renderer.hpp"
#include "renderer/test.hpp"
#include "renderer/variables.hpp"
#include <vulkan/vulkan_core.h>

namespace clz::renderer
{
	void waitForGPU(VkFence fence)
	{
		if (vkWaitForFences(renderer::r_deviceContext.device, 1, &fence, VK_TRUE,
				    UINT64_MAX) != VK_SUCCESS) [[unlikely]]
			clz::log::error("failed to wait for fence");
	}

	void acquireNextImage(VkSemaphore semaphore, uint32_t& rImageIndex)
	{
		const VkResult acquireResult =
		    vkAcquireNextImageKHR(r_deviceContext.device, r_swapchainContext.swapchain,
					  UINT64_MAX, semaphore, VK_NULL_HANDLE, &rImageIndex);
		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR)
		    [[unlikely]]
		{
			r_recreateSwapchain = true;
		}
		else if (acquireResult != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("renderer/mainloop: Failed to acquire next image");
		}
	}

	void resetFence(VkFence fence)
	{
		if (vkResetFences(r_deviceContext.device, 1, &fence) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("Failed to reset fence");
		}
	}

	void startCommandBuffer(VkCommandBuffer commandBuffer)
	{
		if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("Could not reset command buffer mid loop");
		}
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("renderer: midloop: Failed to begin command buffer");
		}
	}

	void recordCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t imageIndex)
	{
		transition_image_layout(
		    r_swapchainContext.images[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
		    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0,
		    VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR,
		    VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT_KHR,
		    VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, commandBuffer);

		const VkRenderingAttachmentInfoKHR colorAttachment{
		    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
		    .pNext = nullptr,
		    .imageView = r_swapchainContext.imageViews[imageIndex],
		    .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		    .clearValue = {{0.0f, 0.0f, 0.0f, 1.0f}}};
		const VkRenderingInfoKHR renderingInfo{
		    .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
		    .pNext = nullptr,
		    .flags = 0,
		    .renderArea = {{0, 0}, r_swapchainContext.extent},
		    .layerCount = 1,
		    .colorAttachmentCount = 1,
		    .pColorAttachments = &colorAttachment};
		vkCmdBeginRendering(commandBuffer, &renderingInfo);

		const VkViewport viewport{
		    .x = 0.0f,
		    .y = 0.0f,
		    .width = static_cast<float>(r_swapchainContext.extent.width),
		    .height = static_cast<float>(r_swapchainContext.extent.height),
		    .minDepth = 0.0f,
		    .maxDepth = 1.0f,
		};
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		const VkRect2D scissor{{0, 0}, r_swapchainContext.extent};
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				  r_pipelineContext.pipeline);
		const VkBuffer vertexBuffers[] = {vertexBuffer};
		constexpr VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
					clz::renderer::r_pipelineContext.layout, 0, 1,
					&descriptorSets[r_currentFrame], 0, nullptr);
		vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

		vkCmdEndRendering(commandBuffer);

		transition_image_layout(
		    r_swapchainContext.images[imageIndex], VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR, 0,
		    VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
		    VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT_KHR, commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("renderer/mainloop: vkEndCommandBuffer failed");
		}
	}

	void submitCommandBuffer(VkCommandBuffer commandBuffer, VkSemaphore imageAvailableSemaphore,
				 VkSemaphore renderFinishedSemaphore, VkFence inFlightFence)
	{
		const VkSemaphoreSubmitInfoKHR waitSemaphore{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.semaphore = imageAvailableSemaphore,
			.value = 0,
			.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
			.deviceIndex = 0
		};


		const VkSemaphoreSubmitInfoKHR signalSemaphore{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.semaphore = renderFinishedSemaphore,
			.value = 0,
			.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT_KHR,
			.deviceIndex = 0
		};


		VkCommandBufferSubmitInfoKHR cmdSubmitInfo{};
		cmdSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
		cmdSubmitInfo.commandBuffer = commandBuffer;

		const VkSubmitInfo2KHR submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.flags = 0,
			.waitSemaphoreInfoCount = 1,
			.pWaitSemaphoreInfos = &waitSemaphore,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &cmdSubmitInfo,
			.signalSemaphoreInfoCount = 1,
			.pSignalSemaphoreInfos = &signalSemaphore
		};

		if (vkQueueSubmit2(r_deviceContext.graphicsQueue, 1, &submitInfo, inFlightFence) !=
		    VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("renderer/mainloop: vkQueueSubmit failed");
		}
	}

	void present(VkSemaphore semaphore, uint32_t imageIndex)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &semaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &r_swapchainContext.swapchain,
		presentInfo.pImageIndices = &imageIndex;

		if (const VkResult result = vkQueuePresentKHR(r_deviceContext.presentQueue, &presentInfo);
		    result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) [[unlikely]]
			r_recreateSwapchain = true;
		else if (result != VK_SUCCESS) [[unlikely]]
			clz::log::error("present failed");
	}
} // namespace clz::renderer
