/*
 * @file initializers.cpp
 * @author curl0z
 * @brief Implementation of initialization
 * of all the context's
 */
#include "renderer/initializers.hpp"
#include "core/logs.hpp"
#include "renderer/devicecontext.hpp"
#include "renderer/framecontext.hpp"
#include "renderer/pipelinecontext.hpp"
#include "renderer/swapchaincontext.hpp"

namespace clz::renderer
{
	std::expected<void, std::string> initDeviceContext()
	{
		auto instanceResult = createInstance();
		if (!instanceResult)
			return std::unexpected(instanceResult.error());

		auto surfacerResult = createSurface();
		if (!surfacerResult)
			return std::unexpected(surfacerResult.error());

		auto gpuResult = selectPhysicalDevice();
		if (!gpuResult)
			return std::unexpected(gpuResult.error());

		auto logicalDeviceResult = createLogicalDevice();
		if (!logicalDeviceResult)
			return std::unexpected(logicalDeviceResult.error());

		clz::log::debug("renderer: initialized device context successfully");

		return {};
	}

	std::expected<void, std::string> initSwapchainContext()
	{
		auto result = createSwapchain();
		if (!result)
			return std::unexpected(result.error());


		clz::log::info("initialized swapchain context successfully");
		return {};
	}

	std::expected<void, std::string> initPipelineContext()
	{
		auto result = createMainPipeline();
		if (!result)
			return std::unexpected(result.error());

		clz::log::info("initialized pipeline context successfully");

		return {};
	}

	std::expected<void, std::string> initFrameContext()
	{
		auto commandPoolResult = createCommandPool();
		if (!commandPoolResult)
			return std::unexpected(commandPoolResult.error());

		auto commandBufferResult = createCommandBuffer();
		if (!commandBufferResult)
			return std::unexpected(commandBufferResult.error());

		auto syncObjectResult = createSyncObjects();
		if (!syncObjectResult)
			return std::unexpected(syncObjectResult.error());


		clz::log::info("initialized frame context successfully");
		return {};
	}
} // namespace clz::renderer