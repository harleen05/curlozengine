#pragma once

#include <cassert>
#include <source_location>
#include <string>

namespace clz
{
	inline void clz_assert(bool condition, std::string_view msg,
			       std::source_location location = std::source_location::current())
	{
#ifdef CLZ_DEBUG

		if (!condition)
		{
			clz::log::error(msg, location);
			assert(false);
		}

#endif
	}
} // namespace clz