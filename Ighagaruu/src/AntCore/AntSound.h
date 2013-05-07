//*****************************************************************************
//-----------------------------------------------------------------------------
// AntSound.h
//
// This is an implementation of a sound manager wrapper. Currently, we
// will use SDL mixer. Currently mimicking some DXUT functionality.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTSOUND_H
#define ANTSOUND_H
#include "Ant.h"


//-----------------------------------------------------------------------------
// AntSound
//-----------------------------------------------------------------------------
class AntSound
{
public:
	AntSound(std::wstring strWaveFileName);
	bool IsSoundPlaying();
	void Reset();
	void Play(DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lVolume = 0, LONG lFrequency = -1, LONG lPan = 0);
	void Stop();
	~AntSound();
	void* music;
};

//-----------------------------------------------------------------------------
// AntSoundManager
//-----------------------------------------------------------------------------
class AntSoundManager
{
	#define BUFFER 1024
	#define W 640 /* NEVER make this be less than BUFFER! */
	#define H 480
	#define H2 (H/2)
	#define H4 (H/4)
	#define Y(sample) (((sample)*H)/4/0x7fff)
public:
	void Create(AntSound** ppSound, std::wstring strWaveFileName);

	template<typename T>
	void Create(AntSound** ppSound, T strWaveFileName, DWORD dwCreationFlags = 0)
	{
		Create(ppSound, std::wstring(strWaveFileName));
	}
	void Initialize();
	template<typename T,typename V>
	void Initialize(T a, V b) { Initialize(); };
	~AntSoundManager();
};

#endif
