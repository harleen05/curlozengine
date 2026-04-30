/**
* @file main.cpp
 * @author curl0z
 * @brief Entry point for Curloz Engine.
 *
 * Initializes all subsystems in order, runs the main loop,
 * and shuts everything down cleanly on exit.
 *
 * @note Subsystems must be initialized in dependency order —
 * config first, then everything that reads from it.
 */

#include "core/logs.hpp"
#include "config/config.hpp"

int main()
{
	// Initialize config first — all subsystems read from it
	clz::config::init();
	if (clz::log::errorOccurred()) return 1;
	clz::config::printAppName();
	clz::config::printAppVersion();

	clz::log::info("Exiting successfully");
	return 0;
}