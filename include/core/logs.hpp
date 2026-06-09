/**
 * @file logs.hpp
 * @author curl0z
 * @brief Simple logging utility for Curloz Engine.
 *
 * Provides info, warn and error logging via std::println.
 * Errors set hadError to true which can be checked by the caller.
 */

#pragma once

#include <print>
#include <source_location>
#include <string_view>

namespace clz::log
{
	/// @brief ANSI color codes for terminal output.
	namespace color
	{
		constexpr const char* reset = "\033[0m";
		constexpr const char* red = "\033[31m";
		constexpr const char* yellow = "\033[33m";
		constexpr const char* blue = "\033[34m";
		constexpr const char* green = "\033[32m";
	} // namespace color

	/// @brief Set to true when error() is called. Never resets automatically.
	inline bool hadError = false;

	/**
	 * @brief Are basic logs or informational messages
	 * @param info string_view type information
	 * @param loc location of caller
	 */
	inline void info(std::string_view info,
			 std::source_location loc = std::source_location::current())
	{
		std::println("{}[INFO]{} {}", color::green, color::reset, info);
	}

	/**
	 * @brief Logs a warning message.
	 * @param warn Warning.
	 */
	inline void warn(std::string_view warn,
			 std::source_location loc = std::source_location::current())
	{
		std::println("{}[WARN]{} {}:Line {}: {}", color::yellow, color::reset,
			     loc.function_name(), loc.line(), warn);
	}

	/**
	 * @brief Logs a debug message.
	 * @param debug message
	 */
	inline void debug(std::string_view debug,
			  std::source_location loc = std::source_location::current())
	{
		std::println("{}[DEBUG]{} {}:Line {}: {}", color::blue, color::reset,
			     loc.function_name(), loc.line(), debug);
	}

	/**
	 * @brief Logs an error message and sets hadError to true.
	 * @param error The error
	 */
	inline void error(std::string_view error,
			  std::source_location loc = std::source_location::current())
	{
		hadError = true;
		std::println("{}[ERROR]{} {}:{}:Line {}: {}", color::red, color::reset,
			     loc.file_name(), loc.function_name(), loc.line(), error);
	}

	/**
	 * @brief Returns true if an error has been logged.
	 */
	inline bool errorOccurred()
	{
		return hadError;
	}

} // namespace clz::log