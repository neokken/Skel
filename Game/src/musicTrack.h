#pragma once

class MusicTrack
{
public:
	MusicTrack() = default;
	~MusicTrack();

	void Init(irrklang::ISoundEngine* soundEngine);

	void PlayTrack(const char* path);

	void SetVolume(irrklang::ik_f32 volume);

	void Pause(bool value);
private:
	irrklang::ISoundEngine* soundEngine{ nullptr };
	irrklang::ISound* mainTrack{ nullptr };

	irrklang::ik_f32 volume{ 1.f };
};

