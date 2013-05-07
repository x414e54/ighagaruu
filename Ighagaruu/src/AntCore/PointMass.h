//*****************************************************************************
//-----------------------------------------------------------------------------
// PointMass.h 
//
// Class for a point mass/particle or whatever. It has a position, velocity,
// acceleration, force accumulator, and a mass. It is considered to have no orienation.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef POINTMASS_H
#define POINTMASS_H

#include "PhysicsOne.h"

class PointMass
{
public:
	PointMass();
	void Calculate(float time);
	void SetPosition(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	void SetAcceleration(float x, float y, float z);
	void AddForce(POVector3 force);
	void SetMass(float m);
	void SetInverseMass(float im);
	void ClearForce();
	POVector3* GetPosition() { return &_position; }
	POVector3 _position;
	POVector3 _velocity;
	POVector3 _acceleration;
	POVector3 _force;
	float _inverseMass;
};

#endif