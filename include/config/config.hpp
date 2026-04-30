/**
 * @file config.hpp
 * @author curl0z
 * @brief Engine configuration system.
 *
 * Loads and parses engine.toml at startup and provides typed
 * getters for reading configuration values by section and key.
 *
 * @note The init of this subsystem must be called "First", ie. before any other's init is called
 * Usage:
 * @code
 * clz::config::init();
 * int width = clz::config::getInt("window", "width", 1920);
 * std::string name = clz::config::getString("engine", "name", "Unknown");
 * @endcode
 */

#pragma once

#include <string>

namespace clz::config
{
	/**
	 * @brief Loads and parses config/engine.toml.
	 *
	 * Must be called before any getter. Logs an error and returns
	 * early if the file is missing or malformed.
	 */
	void init();

	/**
	 * @brief Reads an integer value from the config.
	 * @param section TOML section name (e.g. "window").
	 * @param key     Key within the section (e.g. "width").
	 * @param defaultVal Value returned if section or key is missing.
	 * @return The integer value or defaultVal.
	 */
	int getInt(const char* section, const char* key, int defaultVal);

	/**
	 * @brief Reads a float value from the config.
	 * @param section TOML section name.
	 * @param key     Key within the section.
	 * @param defaultVal Value returned if section or key is missing.
	 * @return The float value or defaultVal.
	 */
	float getFloat(const char* section, const char* key, float defaultVal);

	/**
	 * @brief Reads a boolean value from the config.
	 * @param section TOML section name.
	 * @param key     Key within the section.
	 * @param defaultVal Value returned if section or key is missing.
	 * @return The bool value or defaultVal.
	 */
	bool getBool(const char* section, const char* key, bool defaultVal);

	/**
	 * @brief Reads a string value from the config.
	 * @param section TOML section name.
	 * @param key     Key within the section.
	 * @param defaultVal Value returned if section or key is missing.
	 * @return The string value or defaultVal.
	 */
	std::string getString(const char* section, const char* key, const char* defaultVal);

	/**
	 * @brief Logs the engine name from config.
	 */
	void printAppName();

	/**
	 * @brief Logs the engine version from config as major.minor.patch.
	 */
	void printAppVersion();

} // namespace clz::config