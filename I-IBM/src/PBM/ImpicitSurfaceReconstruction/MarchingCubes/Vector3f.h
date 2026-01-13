/* File Name:   Vector3f.h
 * Description: Header file defining Vector3f class.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */
#ifndef _VECTOR3F_
#define _VECTOR3F_

namespace MarchingCubes
{

class Vector3f{
public:
	Vector3f();
	Vector3f(const float v[3]);
	Vector3f(float x, float y, float z);
	Vector3f cross(const Vector3f &v) const;
	float dot(const Vector3f &v) const;
	bool equals(const Vector3f v, float tolerance) const;
	const  float * getValue() const;
	void getValue(float &x, float &y, float &z) const;
	float length() const;
	void negate();
	float normalize();
	Vector3f & setValue(const float v[3]);
	Vector3f & setValue(float x, float y, float z);
	float & operator [](int i);
//	const float & operator [](int i);
	Vector3f & operator *=(float d);
	Vector3f & operator /=(float d);
	Vector3f & operator +=(const Vector3f &u);
	Vector3f & operator -=(const Vector3f &u);
	Vector3f operator -() const;
	Vector3f operator *(float d);
//	Vector3f operator *(float d, const Vector3f &v);
	Vector3f operator /(float d);
	Vector3f operator +(const Vector3f &u);
	Vector3f operator -(const Vector3f &u);
	int operator ==(const Vector3f &u);
	int operator !=(const Vector3f &u);

private:
	float v3f[3];
};

}
#endif

