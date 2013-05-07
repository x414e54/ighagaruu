//*****************************************************************************
//-----------------------------------------------------------------------------
// AntCamera.h 
//
// Class for creating cameras of different types, A camera has a position PointMass
// and a pointer to its intended target (if any). 
//
// Currently unfinished
//-----------------------------------------------------------------------------
//*****************************************************************************
#pragma once
#ifndef ANTCAMERA_H
#define ANTCAMERA_H

#include "PhysicsOne.h"
#include "PointMass.h"
#include "Ant.h"

class AntCamera
{
public:
	virtual void Update(float timeDelta) { return; }
	virtual bool Event(UINT msg, UINT p1, UINT p2) { return false; }
	POVector3* GetPosition() { return _position.GetPosition(); }
	POVector3* GetTarget() { return _target->GetPosition(); }
	void SetTarget(PointMass* target) { _target=target; }

protected:
	PointMass _position;
	PointMass* _target;
};

/* Unfinished - not needed in this demo
class AntCameraFirstPerson : public AntCamera
{
}
*/

/* Unfinished - not needed in this demo
class AntCameraSideScroller : public AntCamera
{
public:
	AntCameraSideScroller();
	void Update(float timeDelta);
};
*/

/* Unfinished - not needed in this demo
class AntCameraThirdPerson : public AntCamera
{
}
*/

//-----------------------------------------------------------------------------
// A specific "Follower" camera, that will just update to follow the target around from above.
//-----------------------------------------------------------------------------
class AntCameraFollower : public AntCamera
{
public:
	AntCameraFollower();
	void Update(float timeDelta);
};
#endif
