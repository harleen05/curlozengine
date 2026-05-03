/*
 * @file devicecontext.hpp
 * @author curl0z
 * @brief Contains all the functions for
 * initializing the device contexts.
 * ie the vulkan device handles.
 *
 * @note All the contexts are available in variables.hpp
 */
#pragma once

#include <expected>
#include <string>

namespace clz::renderer
{
	/*
	 * @brief Initializes instance
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 * @note Must be the first handle to be created
	 */
	std::expected<void, std::string> createInstance();

	/*
	 * @brief Creates window surface
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 * @note Must be the called after instance has been created
	 */
	std::expected<void, std::string> createSurface();

	/*
	 * @brief Selects physical device ie GPU
	 * based on whether it supports vulkan API 1.3
	 * and has dynamic rendering support.
	 *
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 * @note Selects the best GPU if multiple GPU's
	 * are present
	 */
	std::expected<void, std::string> selectPhysicalDevice();

	/*
	 * @brief Created logical device handle
	 * @return void if succeeded or logs the error
	 * and returns the error message as string
	 * @note Must be the called after
	 * physical device has been selected
	 */
	std::expected<void, std::string> createLogicalDevice();

	/*
	 * @brief Destroys the instance handle
	 * @note must be last instance to be destroyed
	 */
	void destroyInstance();

	/*
	 * @brief Destroys window surface
	 */
	void destroySurface();

	/*
	 * @brief Destroys the logical device handle
	 */
	void destroyDevice();
} // namespace clz::renderer