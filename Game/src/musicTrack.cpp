#include "precomp.h"
#include "musicTrack.h"

MusicTrack::~MusicTrack()
{
	if (mainTrack)
	{
		mainTrack->drop();
	}
}

void MusicTrack::Init(irrklang::ISoundEngine* _soundEngine)
{
	soundEngine = _soundEngine;
	mainTrack = nullptr;
}

void MusicTrack::PlayTrack(const char* path)
{
	if (mainTrack != nullptr)
	{
		mainTrack->stop();
	}
	mainTrack = soundEngine->play2D(path, true, false, true);
	mainTrack->setVolume(volume);
}

void MusicTrack::SetVolume(irrklang::ik_f32 _volume)
{
	mainTrack->setVolume(_volume);
	volume = _volume;
}

void MusicTrack::Pause(bool value)
{
	if (mainTrack)
	{
		mainTrack->setIsPaused(value);
	}
}
