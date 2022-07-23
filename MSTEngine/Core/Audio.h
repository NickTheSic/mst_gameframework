#pragma once
//#include <alc.h>

class AudioPlayer
{
	public:
	static AudioPlayer* Get();

	protected:
	AudioPlayer();
	~AudioPlayer();

	bool Init();

	//ALCdevice* ALCDevice;
	//ALCcontext* ALCContext;

	bool bSuccessful;
};
