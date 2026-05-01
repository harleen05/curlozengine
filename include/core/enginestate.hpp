/**
 * @file enginestate.hpp
 * @author curl0z
 * @brief Engine runtime state.
 */

#pragma once

#include "logs.hpp"
#include <string>

namespace clz
{
	enum class EngineState
	{
		Running,
		Paused,
		Shutdown
	};

	/// @brief Global engine state. The engine will shut down once enum is set to
	/// EngineState::Shutdown
	inline EngineState g_engineState = EngineState::Running;

	inline void setEngineState(const EngineState state, const std::string_view callerLocation)
	{
		g_engineState = state;
		switch (g_engineState)
		{
		case EngineState::Running:
			clz::log::info("Engine state set to Running by: " +
				       std::string(callerLocation));
		case EngineState::Paused:
			clz::log::info("Engine state set to Paused by: " +
				       std::string(callerLocation));
		case EngineState::Shutdown:
			clz::log::info("Engine state set to Shutdown by: " +
				       std::string(callerLocation));
		}
	}
} // namespace clz