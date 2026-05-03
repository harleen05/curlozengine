#pragma once

#include <expected>
#include <string>
#include <AL/alc.h>

namespace clz::audio
{
	std::expected<void, std::string> initializeOpenAL(ALCdevice*& rp_device, ALCcontext*& rp_context);
	void closeOpenAL(ALCdevice*& rp_device, ALCcontext*& rp_context);
}