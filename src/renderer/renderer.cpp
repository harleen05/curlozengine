/**
 * @file renderer.cpp
 * @author curl0z
 * @brief Implementation of the public header
 * of renderer subsystem
 */
#include "renderer/renderer.hpp"
#include "core/logs.hpp"
#include "renderer/cleaners.hpp"
#include "renderer/initializers.hpp"
#include "renderer/mainloop.hpp"
#include "renderer/swapchaincontext.hpp"
#include "renderer/test.hpp"
#include "renderer/variables.hpp"

namespace clz::renderer
{
	void init()
	{
		if (auto deviceContextResult = initDeviceContext(); !deviceContextResult)
		{
			clz::log::error(deviceContextResult.error());
			return;
		}

		if (auto swapchainContextResult = initSwapchainContext(); !swapchainContextResult)
		{
			clz::log::error(swapchainContextResult.error());
			return;
		}

		if (auto pipelineContextResult = initPipelineContext(); !pipelineContextResult)
		{
			clz::log::error(pipelineContextResult.error());
			return;
		}

		if (auto frameContextResult = initFrameContext(); !frameContextResult)
		{
			clz::log::error(frameContextResult.error());
			return;
		}

		clz::log::debug("initialized all renderer context's");
	}

	void update(const float deltaTime)
	{
		if (r_recreateSwapchain) [[unlikely]]
		{
			clz::log::warn("swapchain out of date, recreating it");
			recreateSwapchainContext();
			r_recreateSwapchain = false;
		}
		waitForGPU(r_frameContext.inFlightFences[r_currentFrame]);
		acquireNextImage(r_frameContext.renderReadySemaphores[r_currentFrame],
				 r_imageIndex);
		if (r_recreateSwapchain) [[unlikely]]
			return;
		resetFence(r_frameContext.inFlightFences[r_currentFrame]);

		startCommandBuffer(r_frameContext.commandBuffer[r_currentFrame]);
		recordCommandBuffer(r_frameContext.commandBuffer[r_currentFrame], r_imageIndex);
		submitCommandBuffer(r_frameContext.commandBuffer[r_currentFrame],
				    r_frameContext.renderReadySemaphores[r_currentFrame],
				    r_frameContext.presentReadySemaphores[r_imageIndex],
				    r_frameContext.inFlightFences[r_currentFrame]);

		present(r_frameContext.presentReadySemaphores[r_imageIndex], r_imageIndex); // Internally can also r_recreateSwapchain = true

		r_currentFrame = (r_currentFrame + 1) % FRAMES_IN_FLIGHT;
	}

	void shutdown()
	{
		vkDeviceWaitIdle(r_deviceContext.device);

		destroyFrameContext();
		destroyPipelineContext();
		destroySwapchainContext();
		destroyDeviceContext();

		clz::log::info("renderer shutdown completed");
	}
} // namespace clz::renderer
