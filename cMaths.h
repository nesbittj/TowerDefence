/*
Jonathan Nesbitt
17/04/14
*/
#pragma once
#include <math.h> //for only magnitude sqrt
				  //TODO: write sqrt method

//TODO: add JVector2

#define PIDIV180 0.017453292f

class JVector3
{
public:
	float x,y,z;

	JVector3();
	JVector3(float _x, float _y, float _z);

	bool operator==(const JVector3& v) const;
	bool operator!=(const JVector3& v) const;
	JVector3& operator-() const;
	JVector3& operator=(const JVector3& v);
	
	JVector3& operator+=(const JVector3& v);
	JVector3& operator-=(const JVector3& v);
	JVector3& operator+=(const float& scalar);
	JVector3& operator-=(const float& scalar);
	JVector3& operator*=(const float& scalar);
	JVector3& operator/=(const float& scalar);

	JVector3& operator+(const JVector3& v);
	JVector3& operator-(const JVector3& v);
	JVector3& operator+(const float& scalar);
	JVector3& operator-(const float& scalar);
	JVector3& operator*(const float& scalar);
	JVector3& operator/(const float& scalar);

	float Dot(const JVector3& v) const;
	float Magnitude() const;
	JVector3* Cross(const JVector3* v) const;
	float Normalise();
	void Zero() { x = 0.f; y = 0.f; z = 0.f; }
	static JVector3 Lerp(JVector3& _v0, JVector3& _v1, const float _t);
};

class JMatrix3
{
public:
	JVector3 x;
	JVector3 y;
	JVector3 z;

	JMatrix3();
	JMatrix3(JVector3* _x, JVector3* _y, JVector3* _z);

	bool operator==(const JMatrix3& M2);
	bool operator!=(const JMatrix3& M2);
	JMatrix3& operator=(const JMatrix3 M2);

	JVector3* Column(const int column) const;
	JMatrix3* Dot(const JMatrix3& M2) const;
	void Dot(JVector3* verts, const int num_verts);
	
	void RotationMatrix(const float theta);
	void AffineRotationMatrix(const float theta, const JVector3& position);
	void TranslationMatrix(const JVector3& v);
	void ScaleMatrix(const float scale);

};

class JPlane
{
public:
	JVector3 normal;
	float d;

	JPlane();
	JPlane(JVector3* u, JVector3* v, JVector3* w);

	void Normalise();
	float PlaneEquationValue(const JVector3* intersect_point) const;
	JVector3* PlaneIntersection(JVector3* p1, JVector3* p2) const;

	static JVector3* PlaneNormal(JVector3* u, JVector3* v, JVector3* w);
};

struct float2 { float x; float y; };

class cMaths
{
public:
	static bool InTriangle(const JVector3* t0, const JVector3* t1, const JVector3* t2, const JVector3* p);
	static int Sign(float number);
	static float PythagorasSqrd(float a, float b);
	static float Pythagoras(float a, float b);
	static void Truncatef(float& value);
	static void Truncatef(float& value, const float threshold);
	static int Round(int num, int multiple);
	static int RoundUp(int num, int multiple);
	static bool InRange(float2 _origin, float2 _target, float _range);
};
