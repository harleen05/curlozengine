#include "audio/audio.hpp"
#include "audio/native.hpp"
#include "core/logs.hpp"

#include <AL/alc.h>

namespace clz::audio
{
	static ALCdevice  *au_device = nullptr;
	static ALCcontext *au_ctx    = nullptr;

	void init()
	{
		initializeOpenAL(au_device, au_ctx);
		clz::log::info("Initialized audio");
	}

	void shutdown()
	{
		closeOpenAL(au_device, au_ctx);
		clz::log::info("Shut down audio");
	}
}