#pragma once

#include <expected>
#include <string>

namespace clz::ecs
{
	std::expected<void, std::string> loadEntities();
}