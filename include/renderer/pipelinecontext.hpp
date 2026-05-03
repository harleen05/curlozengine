/*
 * @file pipelinecontext.hpp
 * @author curl0z
 * @brief Initializes all the handles in the pipeline context
 */
#pragma once

#include <expected>
#include <string>

namespace clz::renderer
{
	/*
	 * @brief Creates the main pipeline
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 */
	std::expected<void, std::string> createMainPipeline();

	/*
	 * @brief Destroys the main pipeline
	 */
	void destroyMainPipeline();
} // namespace clz::renderer