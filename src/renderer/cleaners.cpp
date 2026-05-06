/*
 * @file cleaners.cpp
 * @author curl0z
 * @brief Implementation of all the cleanup functions
 * of all the context's
 */
#include "renderer/cleaners.hpp"

#include "core/logs.hpp"
#include "renderer/devicecontext.hpp"
#include "renderer/framecontext.hpp"
#include "renderer/pipelinecontext.hpp"
#include "renderer/swapchaincontext.hpp"

namespace clz::renderer
{
	void destroyDeviceContext()
	{
		destroyDevice();
		destroySurface();
		destroyInstance();

		clz::log::debug("Destroyed Device Context Successfully");
	}

	void destroySwapchainContext()
	{
		destroySwapchain();
		clz::log::debug("Destroyed Swapchain Context Successfully");
	}

	void destroyPipelineContext()
	{
		destroyMainPipeline();
		clz::log::debug("Destroyed pipeline Context Successfully");
	}

	void destroyFrameContext()
	{
		destroyCommandPool();
		destroySyncObjects();
		clz::log::debug("Destroyed frame Context Successfully");
	}
} // namespace clz::renderer
