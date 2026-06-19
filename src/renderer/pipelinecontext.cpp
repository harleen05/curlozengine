/**
 * @file pipelinecontext.cpp
 * @author curl0z
 * @brief Implementation of the initialization
 * and cleanup of pipeline context
 */
#include "renderer/pipelinecontext.hpp"
#include "core/logs.hpp"
#include "renderer/context.hpp"
#include "renderer/variables.hpp"
#include <fstream>
#include <vector>
#include <vulkan/vulkan.h>

// TEST
#include "renderer/test.hpp"

namespace clz::renderer
{
	std::expected<void, std::string>
	createShaderModules(PipelineContext& rPipelineContext,
			    const std::string_view vertexShaderLocation,
			    const std::string_view fragmentShaderLocation)
	{
		// Loading Shaders
		std::ifstream vertex_file(std::string(vertexShaderLocation),
					  std::ios::ate | std::ios::binary);
		std::ifstream frag_file(std::string(fragmentShaderLocation),
					std::ios::ate | std::ios::binary);
		if (!vertex_file)
		{
			clz::log::error("unable to load file: " +
					std::string(vertexShaderLocation));
		}
		if (!frag_file)
		{
			clz::log::error("unable to load file: " +
					std::string(fragmentShaderLocation));
		}

		auto vertexFileSize = vertex_file.tellg();
		std::vector<char> vertexShaderCode(vertexFileSize);
		vertex_file.seekg(0);
		vertex_file.read(vertexShaderCode.data(), (vertexFileSize));
		vertex_file.close();

		auto fragFileSize = frag_file.tellg();
		std::vector<char> fragShaderCode(fragFileSize);
		frag_file.seekg(0);
		frag_file.read(fragShaderCode.data(), fragFileSize);
		frag_file.close();

		// Creating Shader Module

		VkShaderModuleCreateInfo vertInfo = {};
		vertInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vertInfo.codeSize = vertexShaderCode.size();
		vertInfo.pCode = reinterpret_cast<const uint32_t*>(vertexShaderCode.data());
		if (vkCreateShaderModule(r_deviceContext.device, &vertInfo, nullptr,
					 &r_pipelineContext.vertexShader) != VK_SUCCESS)
		{
			clz::log::error("Could not create vertex shader module" +
					std::string(vertexShaderLocation));
			return std::unexpected("Could not create vertex shader module" +
					       std::string(vertexShaderLocation));
		}

		VkShaderModuleCreateInfo fragInfo = {};
		fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		fragInfo.codeSize = fragShaderCode.size();
		fragInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());
		if (vkCreateShaderModule(r_deviceContext.device, &fragInfo, nullptr,
					 &r_pipelineContext.fragmentShader) != VK_SUCCESS)
		{
			clz::log::error("Could not create fragment shader module" +
					std::string(fragmentShaderLocation));
			return std::unexpected("Could not create vertex shader module" +
					       std::string(fragmentShaderLocation));
		}

		clz::log::debug("created shader modules: " + std::string(vertexShaderLocation) +
				" and " + std::string(fragmentShaderLocation));
		return {};
	}

	std::expected<void, std::string> createMainPipeline()
	{
		// Create shaders modules
		auto shaderModuleResult =
		    createShaderModules(r_pipelineContext, "shaders/triangle.vert.spirv",
					"shaders/triangle.frag.spirv");
		if (!shaderModuleResult) [[unlikely]]
		{
			clz::log::error(shaderModuleResult.error());
			return std::unexpected(shaderModuleResult.error());
		}
		// Shader Create Info
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = r_pipelineContext.vertexShader;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = r_pipelineContext.fragmentShader;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
								  fragShaderStageInfo};

		// Pipeline creation

		std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
							     VK_DYNAMIC_STATE_SCISSOR};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		// TEST START

		createVertexBuffer();
		auto bindingDescription = getVertexBindingDescription();
		auto attributeDescriptions = getVertexAttributeDescriptions();
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
		    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		    .pNext = nullptr,
		    .flags = 0,
		    .vertexBindingDescriptionCount = 1,
		    .pVertexBindingDescriptions = &bindingDescription,
		    .vertexAttributeDescriptionCount =
			static_cast<uint32_t>(attributeDescriptions.size()),
		    .pVertexAttributeDescriptions = attributeDescriptions.data()};

		// TEST END

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(r_swapchainContext.extent.width);
		viewport.height = static_cast<float>(r_swapchainContext.extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = r_swapchainContext.extent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		// rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		// rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		// rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		// rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.lineWidth = 1.0f;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		// multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask =
		    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		    VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		createDescriptorSetLayout();
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
		if (vkCreatePipelineLayout(r_deviceContext.device, &pipelineLayoutInfo, nullptr,
					   &r_pipelineContext.layout) != VK_SUCCESS) [[unlikely]]
		{
			clz::log::error("Could not create pipeline layout");
			return std::unexpected("could not create pipeline");
		}

		VkPipelineRenderingCreateInfo pipelineRenderingCI = {};
		pipelineRenderingCI.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		pipelineRenderingCI.colorAttachmentCount = 1;
		pipelineRenderingCI.pColorAttachmentFormats = &r_swapchainContext.format.format;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = &pipelineRenderingCI;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = r_pipelineContext.layout;
		pipelineInfo.renderPass = VK_NULL_HANDLE;
		pipelineInfo.subpass = 0;

		if (vkCreateGraphicsPipelines(r_deviceContext.device, VK_NULL_HANDLE, 1,
					      &pipelineInfo, nullptr,
					      &r_pipelineContext.pipeline) != VK_SUCCESS)
		{
			clz::log::error("Could not create pipeline");
			return std::unexpected("could not create pipeline");
		}

		clz::log::debug("created main pipeline");

		return {};
	}

	void destroyMainPipeline()
	{
		vkDestroyPipeline(r_deviceContext.device, r_pipelineContext.pipeline, nullptr);
		vkDestroyPipelineLayout(r_deviceContext.device, r_pipelineContext.layout, nullptr);
		// TEST
		destroyVertexBuffer();
		// TEST

		vkDestroyShaderModule(r_deviceContext.device, r_pipelineContext.vertexShader,
				      nullptr);
		vkDestroyShaderModule(r_deviceContext.device, r_pipelineContext.fragmentShader,
				      nullptr);

		clz::log::debug("destroyed main pipeline");
	}
} // namespace clz::renderer
