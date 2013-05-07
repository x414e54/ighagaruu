//*****************************************************************************
//-----------------------------------------------------------------------------
// AntTimedEffect.h 
//
// A class for creating an effect which will either start or expire after a certain
// time limit.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTTIMEDEFFECT_H
#define ANTTIMEDEFFECT_H

class AntTimedEffect
{
public:
	AntTimedEffect(float length=1000.0f) : _length(length), _enabled(false), _timeLeft(0.0f) {}
	bool IsEnabled() { return _enabled; }
	void SetLength(float length) { _length=length; }
	void Start() { _timeLeft=_length; _enabled=true; }
	void Update(float timeDelta) { if (_enabled) { _timeLeft-=timeDelta; if(_timeLeft<=0.0f) { _enabled=false; _timeLeft=0.0f; } } }

protected:
	float _length;
	float _timeLeft;
	bool _enabled;
};

#endif