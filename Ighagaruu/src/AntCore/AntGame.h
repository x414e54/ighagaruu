//*****************************************************************************
//-----------------------------------------------------------------------------
// AntGame.h 
//
// Class that contains information about the game and it's state
//
// There are 3 states a game can be in and at each one it can also be paused.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTGAME_H
#define ANTGAME_H

#define UINT unsigned int
enum ANTGAME_STATE
{
	ANTGAME_STATE_MENU,
	ANTGAME_STATE_PLAYING,
	ANTGAME_STATE_CREDITS,
};




class AntGame
{
public:
	void SetState(ANTGAME_STATE state) { _state=state; }
	ANTGAME_STATE GetState() { return _state; }
	void SetPaused(bool paused) { _paused=paused; }
	bool IsPaused() { return _paused; }
private:
	ANTGAME_STATE _state;
	bool _paused;
};

#endif
