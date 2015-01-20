/*
Jonathan Nesbitt
17/04/14
*/

#include "cMaths.h"

JVector3::JVector3() { x = y = z = 0.0f; }
JVector3::JVector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

bool JVector3::operator==(const JVector3& v) const
{
	if(x == v.x && y == v.y && z == v.z) return true;
	return false;
}

/* uses Jvector3::operator== */
bool JVector3::operator!=(const JVector3& v) const { return !(*this == v); }
JVector3& JVector3::operator-() const { return JVector3(-x,-y,-z); }

/* uses JVector3::operator!= */
JVector3& JVector3::operator=(const JVector3& v)
{
	if(this != &v) { x = v.x; y = v.y; z = v.z; }
	return *this;
}

JVector3& JVector3::operator+=(const JVector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
JVector3& JVector3::operator-=(const JVector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
JVector3& JVector3::operator+=(const float& scalar) { x += scalar; y += scalar; z += scalar; return *this; }
JVector3& JVector3::operator-=(const float& scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
JVector3& JVector3::operator*=(const float& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
JVector3& JVector3::operator/=(const float& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
JVector3& JVector3::operator+(const JVector3& v) { JVector3 result = *this; return result += v; }
JVector3& JVector3::operator-(const JVector3& v) { JVector3 result = *this; return result -= v; }
JVector3& JVector3::operator+(const float& scalar) { JVector3 result = *this; return result += scalar; }
JVector3& JVector3::operator-(const float& scalar) { JVector3 result = *this; return result -= scalar; }
JVector3& JVector3::operator*(const float& scalar) { JVector3 result = *this; return result *= scalar; }
JVector3& JVector3::operator/(const float& scalar) { JVector3 result = *this; return result /= scalar; }

float JVector3::Dot(const JVector3& v) const
{
	return (x*v.x) + (y*v.y) + (z*v.z);
}

float JVector3::Magnitude() const { return sqrt((x*x) + (y*y) + (z*z)); }

/* returns inverse magnitude */
float JVector3::Normalise()
{
	float inverseMag = 1/Magnitude();
	x *= inverseMag; y *= inverseMag; z *= inverseMag;
	return inverseMag;
}

JVector3* JVector3::Cross(const JVector3* v) const
{
	return &JVector3(y*v->z - z*v->y,
					 z*v->x - x*v->z,
					 x*v->y - y*v->x);
}

JVector3 JVector3::Lerp(JVector3& _v0, JVector3& _v1, const float _t)
{
	JVector3 result = JVector3(_v1 - _v0);
	result.Normalise();
	return  result * _t;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

JMatrix3::JMatrix3() { x = JVector3(); y = JVector3(); z = JVector3(); }

JMatrix3::JMatrix3(JVector3* _x, JVector3* _y, JVector3* _z)
{
	x = *_x; y = *_y; z = *_z;
}

/* uses JVector3::operator= */
bool JMatrix3::operator==(const JMatrix3& M2)
{
	if(x == M2.x && y == M2.y && z == M2.z) return true;
	return false;
}

/* uses JMatrix3::operator== */
bool JMatrix3::operator!=(const JMatrix3& M2) { return !(*this == M2); }

/* uses JMatrix3::operator!= and JVector3::operator= */
JMatrix3& JMatrix3::operator=(const JMatrix3 M2)
{
	if(this != &M2) { x = M2.x; y = M2.y; z = M2.z; }
	return *this;
}

/*
returns a JVector3* containing the vaulues of a
given column if a matrix.
if parameter is invalid returns nullptr.
*/
JVector3* JMatrix3::Column(const int column) const
{
	switch(column)
	{
	case 0:	return &JVector3(x.x,y.x,z.x);
	case 1:	return &JVector3(x.y,y.y,z.y);
	case 2:	return &JVector3(x.z,y.z,z.z);
	default: return &JVector3(0.f,0.f,0.f);
	}
}

JMatrix3* JMatrix3::Dot(const JMatrix3& M2) const
{
	JMatrix3 result;

	result.x.x = x.Dot(*M2.Column(0));
	result.x.y = x.Dot(*M2.Column(1));
	result.x.z = x.Dot(*M2.Column(2));

	result.y.x = y.Dot(*M2.Column(0));
	result.y.y = y.Dot(*M2.Column(1));
	result.y.z = y.Dot(*M2.Column(2));

	result.z.x = z.Dot(*M2.Column(0));
	result.z.y = z.Dot(*M2.Column(1));
	result.z.z = z.Dot(*M2.Column(2));

	return &result;
}

void JMatrix3::Dot(JVector3* verts, const int num_verts)
{
	float ansx = 0, ansy = 0, ansz = 0;
	for(int i = 0; i < num_verts; i++)
	{
		ansx = x.Dot(verts[i]);
		ansy = y.Dot(verts[i]);
		ansz = z.Dot(verts[i]);

		verts[i].x = ansx;
		verts[i].y = ansy;
		verts[i].z = ansz;
	}
}

/*
sets current matrix to be used as a rotation
matrix with rotation of theta radians.
uses sin and cos funtions from math.h.
*/
void JMatrix3::RotationMatrix(const float theta)
{
	float l_sin_theta = sinf(theta*PIDIV180);
	float l_cos_theta = cosf(theta*PIDIV180);

	x = JVector3(l_cos_theta,-l_sin_theta,0);
	y = JVector3(l_sin_theta,l_cos_theta,0);
	z = JVector3(0,0,1);
}

void JMatrix3::AffineRotationMatrix(const float theta, const JVector3& position)
{
	float l_sin_theta = sinf(theta*PIDIV180);
	float l_cos_theta = cosf(theta*PIDIV180);
	float l_pos_x = (1 - l_cos_theta)*(position.x - (-l_sin_theta*position.y));
	float l_pos_y = (1 - l_cos_theta)*(position.y - (l_sin_theta*position.x));
	
	x = JVector3(l_cos_theta,-l_sin_theta,l_pos_x);
	y = JVector3(l_sin_theta,l_cos_theta,l_pos_y);
	z = JVector3(0,0,1);
}

void JMatrix3::TranslationMatrix(const JVector3& v)
{
	x = JVector3(1,0,v.x);
	y = JVector3(0,1,v.y);
	z = JVector3(0,0,1);
}

void JMatrix3::ScaleMatrix(const float scale)
{
	x = JVector3(scale,0,0);
	y = JVector3(0,scale,0);
	z = JVector3(0,0,scale);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

JPlane::JPlane() { normal = JVector3(); d = 0.0f; }
JPlane::JPlane(JVector3* u, JVector3* v, JVector3* w)
{
	normal = *PlaneNormal(u,v,w);
	d = -u->Dot(normal);
}

/*
uses JVector3::Normalise() to normalise normal,
the return value of JVector::Normalise() is the
inverse of the magnitude of normal, this is
used to normalise d
*/
void JPlane::Normalise() { d *= normal.Normalise(); }

/*
calculates and returns a plane normal
used to represent a plane equation.
takes perameters, three points of a triangle
*/
JVector3* JPlane::PlaneNormal(JVector3* u, JVector3* v, JVector3* w)
{
	JVector3 v_1(u->x - v->x, u->y - v->y, u->z - v->z);
	JVector3 v_2(u->x - w->x, u->y - w->y, u->z - w->z);
	JVector3 normal_vector = *v_1.Cross(&v_2);
	normal_vector.Normalise();

	return &normal_vector;
}

/*
evaluates perameter intersect_point with plane equation.
used to check if the intersect_point is on the +ve
or -ve side of the plane.
plane equation for a point (xn*x) + (yn*y) + (zn*z) + d = 0
*/
float JPlane::PlaneEquationValue(const JVector3* intersect_point) const
{
	return (normal.x * intersect_point->x)
		 + (normal.y * intersect_point->y)
		 + (normal.z * intersect_point->z) + d;
}

JVector3* JPlane::PlaneIntersection(JVector3* p1, JVector3* p2) const
{
	JVector3 intersection_point;

	JVector3 ray = *p2 - *p1;
	float t = ( -d - normal.Dot(*p1) ) / normal.Dot(ray);

	if(t < 0.0 || t > 1.0)
	{
		intersection_point.x = 999999999999.9f; 
		intersection_point.y = 999999999999.9f;
		intersection_point.z = 999999999999.9f;
	}
	else
	{
		intersection_point.x = p1->x + (ray.x*t);
		intersection_point.y = p1->y + (ray.y*t);
		intersection_point.z = p1->z + (ray.z*t);
	}

	return &intersection_point;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

bool cMaths::InTriangle(const JVector3* t0, const JVector3* t1, const JVector3* t2, const JVector3* p)
{
	JVector3 AB(t1->x - t0->x,t1->y - t0->y,t1->z - t0->z);
	JVector3 BC(t2->x - t1->x,t2->y - t1->y,t2->z - t1->z);
	JVector3 CA(t0->x - t2->x,t0->y - t2->y,t0->z - t2->z);
	
	JVector3 AP(p->x - t0->x,p->y - t0->y,p->z - t0->z);
	JVector3 BP(p->x - t1->x,p->y - t1->y,p->z - t1->z);
	JVector3 CP(p->x - t2->x,p->y - t2->y,p->z - t2->z);

	JVector3 APxAB = *AP.Cross(&AB);
	JVector3 BPxBC = *BP.Cross(&BC);
	JVector3 CPxCA = *CP.Cross(&CA);

	if(APxAB.Dot(BPxBC) >= 0)
		if(BPxBC.Dot(CPxCA) >= 0)
			if(CPxCA.Dot(APxAB) >= 0)
				return true;

	return false;
}

/* returns 1 or -1 depending on sign of number, or 0 if zero */
int cMaths::Sign(float number) { return (number < 0.0f ? -1 : (number > 0.0f ? 1 : 0)); }
float cMaths::PythagorasSqrd(float a, float b) { return a*a + b*b; }
float cMaths::Pythagoras(float a, float b) { return sqrt(PythagorasSqrd(a,b)); }

/* truncate float to nearest integer, up or down */
void cMaths::Truncatef(float& value) { floor(value + 0.5); }

/*
if the value is within the threshold of an integer
the value will be truncated to that integer
*/
void cMaths::Truncatef(float& value, const float threshold)
{
	bool neg = false;
	if(value < 0)
	{
		neg = true;
		value = abs(value);
	}

	int l_int = 0;
	if(value >= 0) l_int = floor(value + 0.5) + 1;

	if(l_int - value < threshold) value = l_int - 1;
	if(l_int - value > 1 - threshold) value = l_int - 1;

	if(neg) value = -value;
}

int cMaths::Round(int num, int multiple)
{
	if(multiple == 0) return num;
	int remainder = num % multiple;
	if(remainder == 0) return num;
	return num - remainder;
}

int cMaths::RoundUp(int num, int multiple)
{
	if(multiple == 0) return num;
	int remainder = num % multiple;
	if(remainder == 0) return num;
	return num + multiple - remainder;
}

/*
return true if taret is inside the range of origin,
return false if not.
TODO: check InRange Prerformance
*/
bool cMaths::InRange(float2 _origin, float2 _target, float _range)
{
	if(abs((int)(_origin.x - _target.x)) > _range) return false;
	if(abs((int)(_origin.y - _target.y)) > _range) return false;
	return true;
}