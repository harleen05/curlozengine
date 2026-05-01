/**
 * @file main.cpp
 * @author curl0z
 * @brief Entry point for Curloz Engine.
 *
 * Initializes all subsystems in dependency order, runs the main loop,
 * and shuts everything down cleanly on exit.
 *
 * @note Subsystem init order matters — config must come first
 * as all other subsystems read from it.
 */

#include "config/config.hpp"
#include "core/enginestate.hpp"
#include "core/logs.hpp"
#include "window/window.hpp"

int main()
{
	/*****************************************************
	 *******************INITIALIZATION********************
	 *****************************************************/

	// Initialize config first. All subsystems depend on it
	clz::config::init();
	if (clz::log::errorOccurred()) [[unlikely]]
		return 1;

	// Print App details
	clz::log::info("Welcome to " + clz::config::getAppName());
	clz::config::printAppVersion();

	// Initialize Window. Should be the first subsystem to initialize
	clz::window::init();
	if (clz::log::errorOccurred()) [[unlikely]]
		return 1;
	clz::log::info("Window initialized");



	/*****************************************************
	 ***************      UPDATE       *******************
	 *****************************************************/
	// Main loop. Runs until g_engineState is set to EngineState::Shutdown
	while (clz::g_engineState == clz::EngineState::Running)
	{
		clz::window::update();
	}

	/*****************************************************
	 ***************      SHUTDOWN       *****************
	 *****************************************************/
	clz::window::shutdown();
	clz::log::info("Exiting successfully");
	return 0;
}