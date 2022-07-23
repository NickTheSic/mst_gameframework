#include "Audio.h"
#include "mstDebug.h"

AudioPlayer* AudioPlayer::Get()
{	
	//static AudioPlayer* Player = new AudioPlayer();
	//return Player;
	return nullptr;
}

AudioPlayer::AudioPlayer()
{
	bSuccessful = Init();
}

bool AudioPlayer::Init()
{
	//ALCDevice = alcOpenDevice(nullptr); // Get defualt device;
	//if (ALCDevice == nullptr)
	//{
	//	dbglog("Failed to open device");
	//	return false;
	//}
	//
	//ALCContext = alcCreateContext(ALCDevice, nullptr);
	//if (!ALCContext)
	//{
	//	dbglog("Failed to create Context");
	//	return false;
	//}
	//
	//if (!alcMakeContextCurrent(ALCContext))
	//{
	//	dbglog("Failed to make current context");
	//	return false;
	//}
	//
	//const ALCchar* Name = nullptr;

	//if ()

	return true;
}

AudioPlayer::~AudioPlayer()
{
	//alcMakeContextCurrent(nullptr);
	//alcDestroyContext(ALCContext);
	//alcCloseDevice(ALCDevice);
}
