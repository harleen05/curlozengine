#include "scripting/luafunctions.hpp"
#include "window/keyboard.hpp"
#include "core/logs.hpp"

namespace clz::script
{
	void registerLuaFunctions(sol::state &rLua)
	{
		// expose engine log to lua
		rLua.set_function("log", [](const std::string& msg)
		{
			clz::log::info(msg);
		});


		// ALl keyboard binds
		rLua["Key"] = rLua.create_table_with(
			"Escape",    static_cast<int>(clz::window::Key::Escape),
			"Space",     static_cast<int>(clz::window::Key::Space),
			"Enter",     static_cast<int>(clz::window::Key::Enter),
			"W",         static_cast<int>(clz::window::Key::W),
			"A",         static_cast<int>(clz::window::Key::A),
			"S",         static_cast<int>(clz::window::Key::S),
			"D",         static_cast<int>(clz::window::Key::D),
			"Up",        static_cast<int>(clz::window::Key::Up),
			"Down",      static_cast<int>(clz::window::Key::Down),
			"Left",      static_cast<int>(clz::window::Key::Left),
			"Right",     static_cast<int>(clz::window::Key::Right)
		);
		rLua.set_function("isKeyPressed", [](int key)
		{
			return clz::window::isPressed(static_cast<clz::window::Key>(key));
		});

		rLua.set_function("isKeyReleased", [](int key)
		{
			return clz::window::isReleased(static_cast<clz::window::Key>(key));
		});
	}
}