#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace clz::renderer
{
	inline VkDescriptorPool r_descriptorPool;
	inline VkDescriptorSetLayout r_descriptorSetLayout;
	inline std::vector<VkDescriptorSet> r_descriptorSets;

}

namespace clz::renderer
{
	bool initDescriptors();
	bool createDescriptors();
	void updateDescriptors(uint32_t currentFrame);
	void destroyDescriptors();
}
