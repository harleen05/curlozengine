/**
 * @file image.cpp
 * @brief Vulkan image layout transition utility implementation.
 */

#include "renderer/image.hpp"

namespace clz::renderer
{
	void transition_image_layout(const VkImage image,
				     const VkImageLayout oldLayout,
				     const VkImageLayout newLayout,
				     const VkAccessFlags2 src_access_mask,
				     const VkAccessFlags2 dst_access_mask,
				     const VkPipelineStageFlags2 src_stage_mask,
				     const VkPipelineStageFlags2 dst_stage_mask,
				     const VkCommandBuffer commandBuffer)
	{
		VkImageMemoryBarrier2 barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
		barrier.srcStageMask = src_stage_mask;
		barrier.dstStageMask = dst_stage_mask;
		barrier.srcAccessMask = src_access_mask;
		barrier.dstAccessMask = dst_access_mask;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange = {
		    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		    .baseMipLevel = 0,
		    .levelCount = 1,
		    .baseArrayLayer = 0,
		    .layerCount = 1,
		};

		VkDependencyInfo dependencyInfo = {};
		dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dependencyInfo.pImageMemoryBarriers = &barrier;
		dependencyInfo.imageMemoryBarrierCount = 1;
		dependencyInfo.dependencyFlags = 0;
		dependencyInfo.pNext = nullptr;

		vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
	}
} // namespace clz::renderer
