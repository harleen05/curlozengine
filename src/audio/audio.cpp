#include "audio/audio.hpp"
#include "audio/au_types.hpp"
#include "audio/native.hpp"
#include "core/logs.hpp"
#include <AL/alc.h>

namespace clz::audio
{
	void init()
	{
		auto result = initializeOpenAL(au_device, au_context);
		if (!result)
		{
			clz::log::error(result.error());
		}
		clz::log::info("Initialized audio");
	}

	void shutdown()
	{
		closeOpenAL(au_device, au_context);
		clz::log::info("Shut down audio");
	}
} // namespace clz::audio
