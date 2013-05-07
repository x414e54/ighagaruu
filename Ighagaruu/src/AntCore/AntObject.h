//*****************************************************************************
//-----------------------------------------------------------------------------
// AntObject.h 
//
// An object class for using with the AntGame, the object is similar to the
// Actor class. The two could be brought together into one class, or some similar
// methods and fields separated into a base class.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTOBJECT_H
#define ANTOBJECT_H

#include "PhysicsOne.h"
#include "PointMass.h"

class AntObject
{
public:
	AntObject(POVector3* pos) { SetPosition(pos->_1,pos->_2, pos->_3); _removeOnUpdate=false;}
	POVector3* GetPosition() { return _center.GetPosition(); }
	float GetOrientation() { return _orientation; }
	void SetOrientation(float orientation) { _orientation=orientation; }
	PointMass* GetCenter() { return &_center; }
	unsigned int GetoId() { return _oId; }
	virtual void Update(float timeDelta) { return; }
	void SetPosition(float x, float y, float z) {  _center.SetPosition(x,y,z); }
	float GetRadius() { return _radius; }
	POSphere GetBoundingSphere() { return POSphere(_center._position._1, _center._position._2, _center._position._3, _radius); }
	bool RemoveOnUpdate() { return _removeOnUpdate; }
	void SetRemoveOnUpdate() { _removeOnUpdate=true; }

protected:
	float _radius;
	bool _removeOnUpdate;
	float _orientation;
	PointMass _center;
	unsigned int _meshId;
	unsigned int _oId;
};

#endif