//-----------------------------------------------------------------------------
// Developed from Game Physics Engine Development by Ian millington
//-----------------------------------------------------------------------------

#include "PointMass.h"

//-----------------------------------------------------------------------------
// Constructor - default all to 0.0f
//-----------------------------------------------------------------------------
PointMass::PointMass()
{
	_position=POVector3(0.0f,0.0f,0.0f);
	_velocity=POVector3(0.0f,0.0f,0.0f);
	_force=POVector3(0.0f,0.0f,0.0f);
	_acceleration=POVector3(0.0f,0.0f,0.0f);
}

//-----------------------------------------------------------------------------
// Update the position of the PointMass using simple "physics"
//-----------------------------------------------------------------------------
void PointMass::Calculate(float time)
{

	/* Update position */
	_position+=_velocity*time;

	/* Update velocity */
	_velocity+=(_acceleration+_force*_inverseMass)*time;

	//if (time<=0.0f) { return; }
	ClearForce();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void PointMass::SetPosition(float x, float y, float z)
{
	_position._1=x;
	_position._2=y;
	_position._3=z;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void PointMass::SetVelocity(float x, float y, float z)
{
	_velocity._1=x;
	_velocity._2=y;
	_velocity._3=z;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void PointMass::SetAcceleration(float x, float y, float z)
{
	_acceleration._1=x;
	_acceleration._2=y;
	_acceleration._3=z;
}

//-----------------------------------------------------------------------------
// AddForce that will be included in the next itteration.
//-----------------------------------------------------------------------------
void PointMass::AddForce(POVector3 force)
{
	_force+=force;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void PointMass::SetMass(float m)
{
	if (m>0)
	{
		_inverseMass=1/m;
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void PointMass::SetInverseMass(float im)
{
	_inverseMass=im;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void PointMass::ClearForce()
{
	_force=0;
}
