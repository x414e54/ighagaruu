//*****************************************************************************
//-----------------------------------------------------------------------------
// PhysicsOne.h 
//
// A bunch of useful structs and functions, for 3D rendering and physics creation
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef PHYSICSONE_H
#define PHYSICSONE_H

#include <math.h>
#include "AntMath.h"

struct POVector3
{
	float _1;
	float _2;
	float _3;
	POVector3(float _1=0.0f, float _2=0.0f, float _3=0.0f) : _1(_1), _2(_2), _3(_3) {}

	POVector3 operator*(const POVector3&v)
	{
		return POVector3(_1*v._1, _2*v._2, _3*v._3);
	}

	void operator*=(const POVector3&v)
	{
		_1*=v._1;
		_2*=v._2;
		_3*=v._3;
	}

	POVector3 operator*(const float scalar)
	{
		return POVector3(_1*scalar, _2*scalar, _3*scalar);
	}

	void operator*=(const float scalar)
	{
		_1*=scalar;
		_2*=scalar;
		_3*=scalar;
	}

	void operator/=(const float scalar)
	{
		(*this)*=(1.0f/scalar);
	}

	POVector3 operator/(const float scalar)
	{
		return (*this)/(1.0f/scalar);
	}

	POVector3 operator+(const POVector3&v)
	{
		return POVector3(_1+v._1, _2+v._2, _3+v._3);
	}

	void operator+=(const POVector3&v)
	{
		_1+=v._1;
		_2+=v._2;
		_3+=v._3;
	}

	POVector3 operator-(const POVector3&v)
	{
		return POVector3(_1-v._1, _2-v._2, _3-v._3);
	}

	void operator-=(const POVector3&v)
	{
		_1-=v._1;
		_2-=v._2;
		_3-=v._3;
	}
};

struct POMatrix3
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;
	POMatrix3(
			float _11=0.0f, float _12=0.0f, float _13=0.0f,
			float _21=0.0f, float _22=0.0f, float _23=0.0f, 
			float _31=0.0f, float _32=0.0f, float _33=0.0f
			) : 
	_11(_11), _12(_12), _13(_13),
	_21(_21), _22(_22), _23(_23),
	_31(_31), _32(_32), _33(_33)
	{}

	void operator+=(const POMatrix3&m)
	{
		_11+=m._11; _21+=m._21; _31+=m._31;
		_12+=m._12; _22+=m._22; _32+=m._32;
		_13+=m._13; _23+=m._23; _33+=m._33;
	}

	POMatrix3 operator+(const POMatrix3&m)
	{
		return POMatrix3(
			_11+=m._11, _21+=m._21, _31+=m._31,
			_12+=m._12, _22+=m._22, _32+=m._32,
			_13+=m._13, _23+=m._23, _33+=m._33
			);
	}

	void operator-=(const POMatrix3&m)
	{
		_11-=m._11; _21-=m._21; _31-=m._31;
		_12-=m._12; _22-=m._22; _32-=m._32;
		_13-=m._13; _23-=m._23; _33-=m._33;
	}

	POMatrix3 operator-(const POMatrix3&m)
	{
		return POMatrix3(
			_11-=m._11, _21-=m._21, _31-=m._31,
			_12-=m._12, _22-=m._22, _32-=m._32,
			_13-=m._13, _23-=m._23, _33-=m._33
			);
	}

	void operator/=(const float scalar)
	{
		(*this)*=(1.0f/scalar);
	}

	POMatrix3 operator/(const float scalar)
	{
		return (*this)*(1.0f/scalar);
	}

	void operator*=(const float scalar)
	{
		_11*=scalar; _12*=scalar; _13*=scalar;
		_21*=scalar; _22*=scalar; _23*=scalar;
		_31*=scalar; _32*=scalar; _33*=scalar;
	}

	void operator*=(const POMatrix3&m)
	{
		_11*=m._11; _21*=m._21; _31*=m._31;
		_12*=m._12; _22*=m._22; _32*=m._32;
		_13*=m._13; _23*=m._23; _33*=m._33;
	}

	POMatrix3 operator*(const float scalar)
	{
		return POMatrix3(
			_11*scalar, _12*scalar, _13*scalar,
			_21*scalar, _22*scalar, _23*scalar,
			_31*scalar, _32*scalar, _33*scalar
			);
	}

	POMatrix3 operator*(const POMatrix3&m)
	{
		return POMatrix3(
			_11*=m._11, _21*=m._21, _31*=m._31,
			_12*=m._12, _22*=m._22, _32*=m._32,
			_13*=m._13, _23*=m._23, _33*=m._33
			);
	}
};

struct POMatrix4
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
	POMatrix4(
			float _11=0.0f, float _12=0.0f, float _13=0.0f, float _14=0.0f,
			float _21=0.0f, float _22=0.0f, float _23=0.0f, float _24=0.0f,
			float _31=0.0f, float _32=0.0f, float _33=0.0f, float _34=0.0f,
			float _41=0.0f, float _42=0.0f, float _43=0.0f, float _44=0.0f
			) : 
	_11(_11), _12(_12), _13(_13), _14(_14),
	_21(_21), _22(_22), _23(_23), _24(_24),
	_31(_31), _32(_32), _33(_33), _34(_34),
	_41(_41), _42(_42), _43(_43), _44(_44)
	{}

	void operator+=(const POMatrix4&m)
	{
		_11+=m._11; _21+=m._21; _31+=m._31; _31+=m._41;
		_12+=m._12; _22+=m._22; _32+=m._32; _32+=m._42;
		_13+=m._13; _23+=m._23; _33+=m._33; _33+=m._43;
		_14+=m._14; _24+=m._24; _34+=m._34; _34+=m._44;
	}

	POMatrix4 operator+(const POMatrix4&m)
	{
		return POMatrix4(
			_11+=m._11, _21+=m._21, _31+=m._31, _31+=m._41,
			_12+=m._12, _22+=m._22, _32+=m._32, _32+=m._42,
			_13+=m._13, _23+=m._23, _33+=m._33, _33+=m._43,
			_14+=m._14, _24+=m._24, _34+=m._34, _34+=m._44
			);
	}

	void operator-=(const POMatrix4&m)
	{
		_11-=m._11; _21-=m._21; _31-=m._31; _31-=m._41;
		_12-=m._12; _22-=m._22; _32-=m._32; _32-=m._42;
		_13-=m._13; _23-=m._23; _33-=m._33; _33-=m._43;
		_14-=m._14; _24-=m._24; _34-=m._34; _34-=m._44;
	}

	POMatrix4 operator-(const POMatrix4&m)
	{
		return POMatrix4(
			_11-=m._11, _21-=m._21, _31-=m._31, _31-=m._41,
			_12-=m._12, _22-=m._22, _32-=m._32, _32-=m._42,
			_13-=m._13, _23-=m._23, _33-=m._33, _33-=m._43,
			_14-=m._14, _24-=m._24, _34-=m._34, _34-=m._44
			);
	}

	void operator/=(const float scalar)
	{
		(*this)*=(1.0f/scalar);
	}

	POMatrix4 operator/(const float scalar)
	{
		return (*this)/(1.0f/scalar);
	}

	void operator*=(const float scalar)
	{
		_11*=scalar; _12*=scalar; _13*=scalar; _14*=scalar;
		_21*=scalar; _22*=scalar; _23*=scalar; _24*=scalar;
		_31*=scalar; _32*=scalar; _33*=scalar; _34*=scalar;
		_41*=scalar; _42*=scalar; _43*=scalar; _44*=scalar;
	}

	void operator*=(const POMatrix4&m)
	{
		_11*=m._11; _21*=m._21; _31*=m._31; _31*=m._41;
		_12*=m._12; _22*=m._22; _32*=m._32; _32*=m._42;
		_13*=m._13; _23*=m._23; _33*=m._33; _33*=m._43;
		_14*=m._14; _24*=m._24; _34*=m._34; _34*=m._44;
	}

	POMatrix4 operator*(const float scalar)
	{
		return POMatrix4(
			_11*scalar, _12*scalar, _13*scalar, _14*scalar,
			_21*scalar, _22*scalar, _23*scalar, _24*scalar,
			_31*scalar, _32*scalar, _33*scalar, _34*scalar,
			_41*scalar, _42*scalar, _43*scalar, _44*scalar
			);
	}

	POMatrix4 operator*(const POMatrix4&m)
	{
		return POMatrix4(
			_11*=m._11, _21*=m._21, _31*=m._31, _31*=m._41,
			_12*=m._12, _22*=m._22, _32*=m._32, _32*=m._42,
			_13*=m._13, _23*=m._23, _33*=m._33, _33*=m._43,
			_14*=m._14, _24*=m._24, _34*=m._34, _34*=m._44
			);
	}
};

struct PORECT
{
	int _left;
	int _top;
	int _right;
	int _bottom;
};

static void Swap(float &a, float &b)
{
	float tmp=a;
	a=b;
	b=tmp;
}

static float POSqrt(float f)
{
	return sqrt(f);
}

static POVector3 POMatrixMultiply(POMatrix3 &m, POVector3 &v)
{
	return (POVector3(
		(m._11*v._1)+(m._12*v._2)+(m._13*v._3),
		(m._21*v._1)+(m._22*v._2)+(m._23*v._3),
		(m._31*v._1)+(m._32*v._2)+(m._33*v._3)
		));
}

static void POMatrixMultiply(POMatrix3 &m, POMatrix3 &m2)
{
	m._11=(m._11*m2._11)+(m._12*m2._21)+(m._13*m2._31);
	m._12=(m._11*m2._12)+(m._12*m2._22)+(m._13*m2._32);
	m._13=(m._11*m2._13)+(m._12*m2._23)+(m._13*m2._33);
	m._21=(m._21*m2._21)+(m._21*m2._21)+(m._21*m2._31);
	m._22=(m._21*m2._22)+(m._21*m2._22)+(m._21*m2._32);
	m._23=(m._21*m2._23)+(m._21*m2._23)+(m._21*m2._33);
	m._31=(m._31*m2._31)+(m._31*m2._21)+(m._31*m2._31);
	m._32=(m._31*m2._32)+(m._31*m2._22)+(m._31*m2._32);
	m._33=(m._31*m2._33)+(m._31*m2._23)+(m._31*m2._33);
}

static void POMatrixMultiply(POMatrix4 &m, POMatrix4 &m2)
{
	m._11=(m._11*m2._11)+(m._12*m2._21)+(m._13*m2._31)+(m._14*m2._41);
	m._12=(m._11*m2._12)+(m._12*m2._22)+(m._13*m2._32)+(m._14*m2._42);
	m._13=(m._11*m2._13)+(m._12*m2._23)+(m._13*m2._33)+(m._14*m2._43);
	m._14=(m._11*m2._14)+(m._12*m2._24)+(m._13*m2._34)+(m._14*m2._44);
	m._21=(m._21*m2._21)+(m._21*m2._21)+(m._21*m2._31)+(m._24*m2._41);
	m._22=(m._21*m2._22)+(m._21*m2._22)+(m._21*m2._32)+(m._24*m2._42);
	m._23=(m._21*m2._23)+(m._21*m2._23)+(m._21*m2._33)+(m._24*m2._43);
	m._24=(m._21*m2._24)+(m._21*m2._24)+(m._21*m2._34)+(m._24*m2._44);
	m._31=(m._31*m2._31)+(m._31*m2._21)+(m._31*m2._31)+(m._34*m2._41);
	m._32=(m._31*m2._32)+(m._31*m2._22)+(m._31*m2._32)+(m._34*m2._42);
	m._33=(m._31*m2._33)+(m._31*m2._23)+(m._31*m2._33)+(m._34*m2._43);
	m._34=(m._31*m2._34)+(m._31*m2._24)+(m._31*m2._34)+(m._34*m2._44);
	m._31=(m._41*m2._31)+(m._41*m2._21)+(m._41*m2._31)+(m._44*m2._41);
	m._32=(m._41*m2._32)+(m._41*m2._22)+(m._41*m2._32)+(m._44*m2._42);
	m._33=(m._41*m2._33)+(m._41*m2._23)+(m._41*m2._33)+(m._44*m2._43);
	m._34=(m._41*m2._34)+(m._41*m2._24)+(m._41*m2._34)+(m._44*m2._44);
}

static float PODeterminant(POMatrix3 *m)
{
	return (
		(m->_11*m->_22*m->_33)-
		(m->_11*m->_32*m->_23)-
		(m->_12*m->_21*m->_33)+
		(m->_12*m->_31*m->_23)+
		(m->_13*m->_21*m->_32)-
		(m->_13*m->_22*m->_31)
		);
}
//////////////////////*
/*
static float PODeterminant(POMatrix4 *m)
{
	return (
		(m->_11*PODeterminant(&POMatrix3(
			m->_22, m->_23, m->_24,
			m->_32, m->_33, m->_34,
			m->_42, m->_43, m->_44
			))) -
		(m->_12*PODeterminant(&POMatrix3(
			m->_21, m->_23, m->_24,
			m->_31, m->_33, m->_34,
			m->_41, m->_43, m->_44
			))) +
		(m->_13*PODeterminant(&POMatrix3(
			m->_21, m->_22, m->_24,
			m->_31, m->_32, m->_34,
			m->_41, m->_42, m->_44
			))) -
		(m->_14*PODeterminant(&POMatrix3(
			m->_21, m->_22, m->_23,
			m->_31, m->_32, m->_33,
			m->_41, m->_42, m->_43
			))));
}*/

static void POMatrixTranspose(POMatrix3 &m)
{
	Swap(m._12,m._21);
	Swap(m._13,m._31);
	Swap(m._32,m._23);
}

static void POMatrixTranspose(POMatrix4 &m)
{
	Swap(m._12,m._22);
	Swap(m._13,m._31);
	Swap(m._14,m._44);
	Swap(m._23,m._32);
	Swap(m._24,m._42);
	Swap(m._34,m._43);
}

static POMatrix3 POMatrixCofactor(POMatrix3 *m)
{
	return (POMatrix3(
		(m->_22*m->_33)-(m->_32*m->_23), -((m->_21*m->_33)-(m->_31*m->_23)), (m->_21*m->_32)-(m->_31*m->_22),
		-((m->_12*m->_33)-(m->_32*m->_13)), (m->_11*m->_33)-(m->_31*m->_13), -((m->_11*m->_32)-(m->_31*m->_12)),
		(m->_12*m->_23)-(m->_22*m->_13), -((m->_11*m->_23)-(m->_21*m->_13)), (m->_11*m->_22)-(m->_21*m->_12)
		));
}

/*static POMatrix4 POMatrixCofactor(POMatrix4 *m)
{
	return (POMatrix4(
		(m->_22*m->_33)-(m->_32*m->_23), -((m->_21*m->_33)-(m->_31*m->_23)), (m->_21*m->_32)-(m->_31*m->_22),
		-((m->_12*m->_33)-(m->_32*m->_13)), (m->_11*m->_33)-(m->_31*m->_13), -((m->_11*m->_32)-(m->_31*m->_12)),
		(m->_12*m->_23)-(m->_22*m->_13), -((m->_11*m->_23)-(m->_21*m->_13)), (m->_11*m->_22)-(m->_21*m->_12)
		));;
}*/

/*static void POInvertMatrix(POMatrix4 &m)
{
	float det = PODeterminant(&m);
	if (det <=0) return;
	POMatrix4 adj=POMatrixCofactor(&m);
	POMatrixTranspose(adj);
	m=adj/det;
}*/

static void POInvertMatrix(POMatrix3 &m)
{
	float det = PODeterminant(&m);
	if (det <=0) return;
	POMatrix3 adj=POMatrixCofactor(&m);
	POMatrixTranspose(adj);
	m=adj/det;
}

static float ScalarProduct(POVector3 &v, POVector3 &v2)
{
	return (
		(v._1*v2._1) +
		(v._2*v2._2) +
		(v._3*v2._3)
		);
}

static void VectorProduct(POVector3 &v, POVector3 &v2)
{
	v._1=(v._2*v2._3) - (v._3*v2._2);
	v._2=(v._3*v2._1) - (v._1*v2._3);
	v._3=(v._1*v2._2) - (v._2*v2._1);
}

static float Magnitude(POVector3 *v)
{
	return POSqrt((v->_1*v->_1)+(v->_2*v->_2)+(v->_3*v->_3));
}

static float Magnitude(POVector3 v)
{
	return Magnitude(&v);
}

static void Normalize(POVector3 &v)
{
	v=v/Magnitude(&v);
}

static float SquareMagnitude(POVector3 *v)
{
	return ((v->_1*v->_1)+(v->_2*v->_2)+(v->_3*v->_3));
}

static float SquareMagnitude(POVector3 v)
{
	return SquareMagnitude(&v);
}

struct POSphere
{
	float _radius;
	POVector3 _pos;
	POSphere(float x, float y, float z, float r) : _radius(r) { _pos=POVector3(x,y,z); }
	POSphere(POVector3 pos, float r) : _radius(r), _pos(pos) {}
};

struct RECT
{
	int x;
	int y;
	int h;
	int w;
	int right;
	int left;
	int top;
	int bottom;
};

static void SetRect(RECT* rect, int x, int y, int h, int w)
{
	rect->x = x;
	rect->y = y;
	rect->h = h;
	rect->w = w;
}

static void OffsetRect(RECT* rect, int x, int y)
{
}

static float CalcAngle(float a1, float a2)
{
	float temp = a2 - a1;
	if (temp >= PI)
	{
		temp = temp - PI - PI;
	} else if (temp <= -PI)
	{
		temp = temp + PI + PI;
	}
	return temp;
}

struct POINT
{
	int x;
	int y;
};
#endif
