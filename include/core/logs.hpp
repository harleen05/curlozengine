/**
 * @file logs.hpp
 * @author curl0z
 * @brief Simple logging utility for Curloz Engine.
 *
 * Provides info, warn and error logging via std::println.
 * Errors set hadError to true which can be checked by the caller.
 *
 * Usage:
 * @code
 * clz::log::info("xyz initialized");
 * clz::log::warn("system is outdated: {}, {}", parm1, parm2);
 * clz::log::error("Failed to create xyz: {}", path);
 *
 * if (clz::log::errorOccurred)
 *         // handle
 * @endcode
 */

#pragma once

#include <print>
#include <string_view>

namespace clz::log
{
	/// @brief Set to true when error() is called. Never resets automatically.
	inline bool hadError = false;

	/**
	 * @brief Are basic logs or informational messages
	 * @param info string_view type information
	 */
	inline void info(std::string_view info)
	{
		std::println("[INFO] {}", info);
	}

	/**
	 * @brief Logs a warning message.
	 * @param warn Warning.
	 */
	inline void warn(std::string_view warn)
	{
		std::println("[WARN] {}", warn);
	}

	/**
	 * @brief Logs an error message and sets hadError to true.
	 * @param error The error
	 */
	inline void error(std::string_view error)
	{
		hadError = true;
		std::println("[ERROR] {}", error);
	}

	/**
	 * @brief Returns true if an error has been logged.
	 */
	inline bool errorOccurred()
	{
		return hadError;
	}

} // namespace clz::log