/*
 * @file mainloop.cpp
 * @author curl0z
 * @brief Implementation of the main loop of the frame
 */
#include "renderer/mainloop.hpp"
#include "core/logs.hpp"
#include "renderer/cleaners.hpp"
#include "renderer/image.hpp"
#include "renderer/initializers.hpp"
#include "renderer/swapchaincontext.hpp"
#include "renderer/variables.hpp"

namespace clz::renderer
{
	void waitForGPU(const VkFence fence)
	{
		if (vkWaitForFences(renderer::r_deviceContext.device, 1, &fence, VK_TRUE,
				    UINT64_MAX) != VK_SUCCESS) [[unlikely]]
			clz::log::error("failed to wait for fence");

		if (vkResetFences(r_deviceContext.device, 1, &fence) != VK_SUCCESS) [[unlikely]]
			clz::log::error("Failed to reset fence");
	}

	void acquireNextImage(const VkSemaphore semaphore, uint32_t& rImageIndex)
	{
		const VkResult acquireResult =
		    vkAcquireNextImageKHR(r_deviceContext.device, r_swapchainContext.swapchain,
					  UINT64_MAX, semaphore, VK_NULL_HANDLE, &rImageIndex);
		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR)
		    [[unlikely]]
		{
			clz::log::warn("Swapchain is outdated, recreating it");
			recreateSwapchain();
		}
		else if (acquireResult != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("renderer/mainloop: Failed to acquire next image");
		}
	}

	void recreateSwapchain()
	{
		vkDeviceWaitIdle(r_deviceContext.device);

		destroySwapchainContext();
		auto swapchainResult = initSwapchainContext();
		if (!swapchainResult) [[unlikely]]
		{
			clz::log::error("Mid loop, failed to recreate swapchain");
		}
	}

	void startCommandBuffer(const VkCommandBuffer commandBuffer)
	{
		if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("Could not reset command buffer mid loop");
		}
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			clz::log::error("midloop: Failed to begin command buffer");
		}
	}

	void recordCommandBuffer(const VkCommandBuffer commandBuffer, const uint32_t imageIndex)
	{
		transition_image_layout(
		    r_swapchainContext.images[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
		    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0,
		    VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR,
		    VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT_KHR,
		    VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, commandBuffer);

		const VkRenderingAttachmentInfoKHR colorAttachment{
		    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
		    .imageView = r_swapchainContext.imageViews[imageIndex],
		    .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		    .clearValue = {{0.0f, 0.0f, 0.0f, 1.0f}}};
		const VkRenderingInfoKHR renderingInfo{
		    .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
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
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

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

	void submitCommandBuffer(const VkCommandBuffer commandBuffer,
				 const VkSemaphore imageAvailableSemaphore,
				 const VkSemaphore renderFinishedSemaphore,
				 const VkFence inFlightFence)
	{
		const VkSemaphoreSubmitInfoKHR waitSemaphore{
		    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
		    .semaphore = imageAvailableSemaphore,
		    .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR};

		const VkSemaphoreSubmitInfoKHR signalSemaphore{
		    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
		    .semaphore = renderFinishedSemaphore,
		    .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT_KHR};

		VkCommandBufferSubmitInfoKHR cmdSubmitInfo{};
		cmdSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
		cmdSubmitInfo.commandBuffer = commandBuffer;

		const VkSubmitInfo2KHR submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
						  .waitSemaphoreInfoCount = 1,
						  .pWaitSemaphoreInfos = &waitSemaphore,
						  .commandBufferInfoCount = 1,
						  .pCommandBufferInfos = &cmdSubmitInfo,
						  .signalSemaphoreInfoCount = 1,
						  .pSignalSemaphoreInfos = &signalSemaphore};

		if (vkQueueSubmit2(r_deviceContext.graphicsQueue, 1, &submitInfo, inFlightFence) !=
		    VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("renderer/mainloop: vkQueueSubmit failed");
		}
	}

	void present(VkSemaphore semaphore, uint32_t imageIndex)
	{
		const VkPresentInfoKHR presentInfo{.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
						   .waitSemaphoreCount = 1,
						   .pWaitSemaphores = &semaphore,
						   .swapchainCount = 1,
						   .pSwapchains = &r_swapchainContext.swapchain,
						   .pImageIndices = &imageIndex};

		if (vkQueuePresentKHR(r_deviceContext.presentQueue, &presentInfo) != VK_SUCCESS)
		    [[unlikely]]
		{
			clz::log::error("renderer/mainloop: vkQueuePresent failed");
		}
	}
} // namespace clz::renderer
