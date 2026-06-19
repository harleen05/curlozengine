#pragma once

#include "core/logs.hpp"
#include <cassert>
#include <source_location>
#include <string_view>

namespace clz
{
	inline void CLZ_ASSERT(const bool condition, std::string_view msg,
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
