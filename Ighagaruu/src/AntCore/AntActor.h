//*****************************************************************************
//-----------------------------------------------------------------------------
// AntActor.h 
//
// Class that contains information about a player or ai player that
// can then be controlled.
//
// An AntActor is simply a PointMass, with an orientation in the y direction only,
// this can all be extended to have more than one pointMass, to make a collection
// or some other kind of object to donate its position.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTACTOR_H
#define ANTACTOR_H

#include "PhysicsOne.h"
#include "PointMass.h"

class AntActor
{
public:
	//AntActor(unsigned int pId) { _pId=pId; }
	POVector3* GetPosition() { return _center.GetPosition(); }
	float GetOrientation() { return _orientation; }
	PointMass* GetCenter() { return &_center; }
	unsigned int GetpId() { return _pId; }
	virtual void Update(float timeDelta) { return; }
	void SetPosition(float x, float y, float z) {  _center.SetPosition(x,y,z); }
	float GetRadius() { return _radius; }
	POSphere GetBoundingSphere() { return POSphere(_center._position._1, _center._position._2, _center._position._3, _radius); }

protected:
	float _radius;
	float _orientation;
	PointMass _center;
	unsigned int _meshId;
	unsigned int _pId;
};

#endif