#include "AntSound.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mixer.h>

//-----------------------------------------------------------------------------
// AntSound
//-----------------------------------------------------------------------------
AntSound::AntSound(std::wstring strWaveFileName)
{
	/* load the song */
	std::string str = Convert(strWaveFileName);
	if(!(music=Mix_LoadMUS(str.c_str())))
	{
		fprintf(stderr,"LoadAudioFailed(%s)\n", Mix_GetError());
		return;
	}
}
bool AntSound::IsSoundPlaying() { return false; }
void AntSound::Reset() {}
void AntSound::Play(DWORD dwPriority, DWORD dwFlags, LONG lVolume, LONG lFrequency, LONG lPan) {
	//Mix_ResumeMusic();
	if(Mix_PlayMusic((Mix_Music*)music, 1)==-1)
	{
		fprintf(stderr,"PlayAudioFailed(%s)\n", Mix_GetError());
		return;
	}
	Mix_VolumeMusic(lVolume);
}
void AntSound::Stop() {
	Mix_PauseMusic();
}

AntSound::~AntSound()
{
	Mix_FreeMusic((Mix_Music*)music);
}

//-----------------------------------------------------------------------------
// AntSoundManager
//-----------------------------------------------------------------------------
void AntSoundManager::Create(AntSound** ppSound, std::wstring strWaveFileName)
{
	*ppSound = new AntSound(strWaveFileName);
}

void AntSoundManager::Initialize() {
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		fprintf(stderr,"OpenAudioFailed(%s)\n",  SDL_GetError());
		return;
	}

	for (int i=0; i<SDL_GetNumAudioDrivers(); i++)
	{
		fprintf(stderr,"OpenAudioPassed(%s)\n", SDL_GetAudioDeviceName(i, 0));
	}

	/* initialize sdl mixer, open up the audio device */
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,BUFFER)<0)
	{
		fprintf(stderr,"OpenAudioFailed(%s)\n", Mix_GetError());
		return;
	}
}

AntSoundManager::~AntSoundManager()
{
	Mix_CloseAudio();
}

