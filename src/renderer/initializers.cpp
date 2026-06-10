/**
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
		if (auto instanceResult = createInstance(); !instanceResult)
			return std::unexpected(instanceResult.error());

		if (auto debugMessengerResult = createDebugMessenger(); !debugMessengerResult)
			return std::unexpected(debugMessengerResult.error());

		if (auto surfacerResult = createSurface(); !surfacerResult)
			return std::unexpected(surfacerResult.error());

		if (auto gpuResult = selectPhysicalDevice(); !gpuResult)
			return std::unexpected(gpuResult.error());

		if (auto logicalDeviceResult = createLogicalDevice(); !logicalDeviceResult)
			return std::unexpected(logicalDeviceResult.error());

		clz::log::debug("renderer: initialized device context successfully");

		return {};
	}

	std::expected<void, std::string> initSwapchainContext()
	{
		if (auto result = createSwapchain(); !result)
			return std::unexpected(result.error());

		clz::log::debug("initialized swapchain context successfully");
		return {};
	}

	std::expected<void, std::string> initPipelineContext()
	{
		if (auto result = createMainPipeline(); !result)
			return std::unexpected(result.error());

		clz::log::debug("initialized pipeline context successfully");

		return {};
	}

	std::expected<void, std::string> initFrameContext()
	{
		if (auto commandPoolResult = createCommandPool(); !commandPoolResult)
			return std::unexpected(commandPoolResult.error());

		if (auto commandBufferResult = createCommandBuffer(); !commandBufferResult)
			return std::unexpected(commandBufferResult.error());

		if (auto syncObjectResult = createSyncObjects(); !syncObjectResult)
			return std::unexpected(syncObjectResult.error());

		clz::log::debug("initialized frame context successfully");
		return {};
	}
} // namespace clz::renderer