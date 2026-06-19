#pragma once 

#include <vulkan/vulkan.h>
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "renderer/variables.hpp"
#include "core/logs.hpp"
#include <array>
#include <memory.h>
#include "math/mat4x4.hpp"
#include "math/localtransform.hpp"
#include "core/time.hpp"
#include <cmath>


struct Vertex {
	clz::math::vec2 pos;
    	clz::math::vec3 color;
};

const std::array<Vertex, 3> vertices = {{
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
    	{{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}
}};

inline VkBuffer vertexBuffer;
inline VkDeviceMemory vertexBufferMemory;

inline VkVertexInputBindingDescription getVertexBindingDescription() 
{
    	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    	return bindingDescription;
}
inline std::array<VkVertexInputAttributeDescription, 2> getVertexAttributeDescriptions() {
    	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
	
    	return attributeDescriptions;

}

inline uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
{
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(clz::renderer::r_deviceContext.gpu, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
            	if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
                	return i;
        }

	clz::log::error("failed to find suitable memory type!");
	return UINT32_MAX;
}

inline void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) 
{
	auto& device = clz::renderer::r_deviceContext.device;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
	{
		clz::log::error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
	{
		clz::log::error("could not allocate memory");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

inline void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
{
	auto& device = clz::renderer::r_deviceContext.device;
	auto& commandPool = clz::renderer::r_frameContext.commandPool;
	auto* graphicsQueue = clz::renderer::r_deviceContext.graphicsQueue;


        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
 }



inline void createVertexBuffer()
{
	auto& device = clz::renderer::r_deviceContext.device;

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);


	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
	vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

}

inline void destroyVertexBuffer()
{
	vkDestroyBuffer(clz::renderer::r_deviceContext.device, vertexBuffer, nullptr);
	vkFreeMemory(clz::renderer::r_deviceContext.device, vertexBufferMemory, nullptr);
}




// Descriptor sets

inline VkDescriptorSetLayout descriptorSetLayout;
inline std::array<VkBuffer, 2> uniformBuffers;
inline std::array<VkDeviceMemory, 2> uniformBuffersMemory;
inline std::array<void*, 2> uniformBuffersMapped;

struct UniformBufferObject 
{
	clz::math::mat4 model;	
};
UniformBufferObject UBO;


inline void createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(clz::renderer::r_deviceContext.device,
				&layoutInfo, nullptr, &descriptorSetLayout)
					!= VK_SUCCESS)
	{
		clz::log::error("Could not initialize descriptor set layout");
	}
}

inline void createUniformBuffer()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	for (size_t i = 0; i < 2; i++) 
	{
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
				uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(clz::renderer::r_deviceContext.device, uniformBuffersMemory[i], 
				0, bufferSize, 0, &uniformBuffersMapped[i]);
    	}
}

inline void destroyDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(clz::renderer::r_deviceContext.device, descriptorSetLayout, nullptr);
}

inline void destroyUniformBuffer()
{
	for (size_t i = 0; i < 2; i++) 
	{
		vkDestroyBuffer(clz::renderer::r_deviceContext.device, uniformBuffers[i], nullptr);
		vkFreeMemory(clz::renderer::r_deviceContext.device, uniformBuffersMemory[i], nullptr);
	}
}

inline void updateUniformBuffer(const uint8_t currentFrame)
{
	clz::math::makeIdentity(UBO.model);
	clz::math::scale(UBO.model, clz::math::scalar_product(clz::math::vec3(1.0f, 1.0f, 1.0f), 
				(float)sin(clz::time::getTotalElapsedTime())));
	clz::math::translate(UBO.model, clz::math::scalar_product(clz::math::vec3(1.0f, 0.0f, 0.0f),
				(float)sin(clz::time::getTotalElapsedTime())));

	memcpy(uniformBuffersMapped[currentFrame], &UBO, sizeof(UBO));
}
