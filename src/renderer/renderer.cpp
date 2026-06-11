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
		waitForGPU(r_frameContext.inFlightFences[r_currentFrame]);
		acquireNextImage(r_frameContext.imageAvailableSemaphores[r_currentFrame],
				 r_imageIndex);
		startCommandBuffer(r_frameContext.commandBuffer[r_imageIndex]);
		recordCommandBuffer(r_frameContext.commandBuffer[r_imageIndex], r_imageIndex);
		submitCommandBuffer(r_frameContext.commandBuffer[r_imageIndex],
				    r_frameContext.imageAvailableSemaphores[r_currentFrame],
				    r_frameContext.renderFinishedSemaphores[r_imageIndex],
				    r_frameContext.inFlightFences[r_currentFrame]);
		present(r_frameContext.renderFinishedSemaphores[r_imageIndex], r_imageIndex);

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