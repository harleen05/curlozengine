/**
* @file scripting.cpp
 * @author curl0z
 * @brief Lua scripting subsystem implementation.
 */

#include "scripting/scripting.hpp"
#include "core/logs.hpp"
#include "scripting/luafunctions.hpp"

#include <sol/sol.hpp>

namespace clz::script
{
	/// @brief Global Lua state.
	inline sol::state sc_lua;

	void init()
	{
		// load standard lua libraries
		sc_lua.open_libraries(
			sol::lib::base,
			sol::lib::math,
			sol::lib::string,
			sol::lib::table
		);

		registerLuaFunctions(sc_lua);

		clz::log::info("Scripting subsystem initialized");
	}

	void runScript(const std::string& path)
	{
		auto result = sc_lua.script_file(path);
		if (!result.valid())
		{
			sol::error err = result;
			clz::log::error("Script error: " + std::string(err.what()));
		}
	}

} // namespace clz::scripting