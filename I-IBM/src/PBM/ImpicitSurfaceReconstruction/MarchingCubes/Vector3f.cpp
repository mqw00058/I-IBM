/* File Name:   Vector3f.cpp
 * Description: Implementation of Vector3f class
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */
#include <iostream>
#include <math.h>
#include "Vector3f.h"
using namespace std;

namespace MarchingCubes{

Vector3f::Vector3f(){}

Vector3f::Vector3f(const float v[3]){
    for (int i = 0; i < 3; i ++)
	v3f[i] = v[i];
}

Vector3f::Vector3f(float x, float y, float z){
    v3f[0] = x;
    v3f[1] = y;
    v3f[2] = z;
}

Vector3f Vector3f::cross(const Vector3f &v) const{
    Vector3f result(v3f[1] * v.getValue()[2] - v3f[2] * v.getValue()[1],
                    v3f[2] * v.getValue()[0] - v3f[0] * v.getValue()[2],
                    v3f[0] * v.getValue()[1] - v3f[1] * v.getValue()[0]);
    return result;
}

float Vector3f::dot(const Vector3f &v) const{
    float result = v3f[0] * v.getValue()[0] + v3f[1] * v.getValue()[1] + v3f[2] * v.getValue()[2];
    return result;
}

bool Vector3f::equals(const Vector3f v, float tolerance) const{
    return ((v3f[0] - v.getValue()[0]) * (v3f[0] - v.getValue()[0]) +
            (v3f[1] - v.getValue()[1]) * (v3f[1] - v.getValue()[1]) +
            (v3f[2] - v.getValue()[2]) * (v3f[2] - v.getValue()[2]) <= tolerance);
}

const float * Vector3f::getValue() const{
    return v3f;
}

void Vector3f::getValue(float &x, float &y, float &z) const{
    x = v3f[0];
    y = v3f[1];
    z = v3f[2];
}

float Vector3f::length() const{
    return sqrt(v3f[0] * v3f[0] +
		v3f[1] * v3f[1] +
		v3f[2] * v3f[2]);
}

void Vector3f::negate(){
    v3f[0] = -v3f[0];
    v3f[1] = -v3f[1];
    v3f[2] = -v3f[2];
}

float Vector3f::normalize(){
    float length = this->length();
    if (length != 0.0){
	v3f[0] /= length;
	v3f[1] /= length;
	v3f[2] /= length;
    }
    return length;
}

Vector3f & Vector3f::setValue(const float v[]){
    for (int i = 0; i < 3; i ++)
	v3f[i] = v[i];

    return *this;
}

Vector3f & Vector3f::setValue(float x, float y, float z){
    v3f[0] = x;
    v3f[1] = y;
    v3f[2] = z;

    return *this;
}

float & Vector3f::operator[](int i){
    return v3f[i];
}

/* const float & Vector3f::operator[](int i){
	return v3f[i];
} */

Vector3f & Vector3f::operator*=(float d){
    v3f[0] *= d;
    v3f[1] *= d;
    v3f[2] *= d;
    return *this;
}

Vector3f & Vector3f::operator/=(float d){
    v3f[0] /= d;
    v3f[1] /= d;
    v3f[2] /= d;
    return *this;
}

Vector3f & Vector3f::operator+=(const Vector3f &u){
    v3f[0] += u.getValue()[0];
    v3f[1] += u.getValue()[1];
    v3f[2] += u.getValue()[2];
    return *this;
}

Vector3f & Vector3f::operator-=(const Vector3f &u){
    v3f[0] -= u.getValue()[0];
    v3f[1] -= u.getValue()[1];
    v3f[2] -= u.getValue()[2];
    return *this;
}

Vector3f Vector3f::operator-() const{
    Vector3f newV3f(-v3f[0], -v3f[1], -v3f[2]);
    return newV3f;
}

Vector3f Vector3f::operator*(float d){
    Vector3f newV3f(v3f[0] * d, v3f[1] * d, v3f[2] * d);
    return newV3f;
}

/* Vector3f Vector3f::operator *(float d, const Vector3f &v){
	Vector3f newV3f(v.getValue()[0] * d, v.getValue()[1] * d, v.getValue()[2] * d);
	return newV3f;
} */

Vector3f Vector3f::operator/(float d){
    Vector3f newV3f(v3f[0] / d, v3f[1] / d, v3f[2] / d);
    return newV3f;
}

Vector3f Vector3f::operator+(const Vector3f &u){
    Vector3f newV3f(v3f[0] + u.getValue()[0], v3f[1] + u.getValue()[1], v3f[2] + u.getValue()[2]);
    return newV3f;
}

Vector3f Vector3f::operator-(const Vector3f &u){
    Vector3f newV3f(v3f[0] - u.getValue()[0], v3f[1] - u.getValue()[1], v3f[2] - u.getValue()[2]);
    return newV3f;
}

int Vector3f::operator==(const Vector3f &u){
    return (v3f[0] == u.getValue()[0]) && (v3f[1] == u.getValue()[1]) && (v3f[2] == u.getValue()[2]) ? 1 : 0;
}

int Vector3f::operator!=(const Vector3f &u){
    return (v3f[0] == u.getValue()[0]) && (v3f[1] == u.getValue()[1]) && (v3f[2] == u.getValue()[2]) ? 0 : 1;
}

// Test Vector3f
/* int main(int argc, char** argv){
    float v1[3] = {0.33, 0.93, 0.65};
    float v2[3] = {0.74, 0.32, 0.48};

    float multipler = 0.4;
    float divider = 0.9;

    int ignore;

    Vector3f v3f1(v1), v3f2(v2);

    // Test operator []:
    qDebug() << "v1 = (" << v3f1[0] << ", " << v3f1[1] << ", " << v3f1[2] << ")" << endl;
    qDebug() << "v2 = (" << v3f2[0] << ", " << v3f2[1] << ", " << v3f2[2] << ")" << endl;
    qDebug() << "v1.length = " << v3f1.length() << endl;
    qDebug() << "v2.length = " << v3f2.length() << endl;
    qDebug() << "v1 equals v2 ? : " << v3f1.equals(v3f2, 0.01) << endl;
    qDebug() << "-v1 = (" << (-v3f1)[0] << ", " << (-v3f1)[1] << ", " << (-v3f1)[2] << ")" << endl;
    v3f1.negate();
    qDebug() << "v1.negate() = (" << v3f1[0] << ", " << v3f1[1] << ", " << v3f1[2] << ")" << endl;
    v3f1.negate();
    qDebug() << endl;

    // Test setValue:
    qDebug() << "swap v1 and v2: " << endl;
    float x, y, z;
    v3f1.getValue(x, y, z);
    v3f1.setValue(v3f2.getValue());
    v3f2.setValue(x, y, z);
    qDebug() << "v1 = (" << v3f1[0] << ", " << v3f1[1] << ", " << v3f1[2] << ")" << endl;
    qDebug() << "v2 = (" << v3f2[0] << ", " << v3f2[1] << ", " << v3f2[2] << ")" << endl;
    qDebug() << endl;

    // Test add:
    qDebug() << "v1 + v2 = (";
    Vector3f v3f3 = v3f1 + v3f2;
    qDebug() << v3f3[0] << ", " << v3f3[1] << ", " << v3f3[2] << ")" << endl;
    Vector3f v3f1_bak(v3f1.getValue());
    qDebug() << "v1 += v2, v1 = (";
    v3f1 += v3f2;
    qDebug() << v3f1[0] << ", " << v3f1[1] << ", " << v3f1[2] << ")" << endl;
    qDebug() << endl;

    // Test subtract:
    qDebug() << "v1 - v2 = (";
    Vector3f v3f4 = v3f1_bak - v3f2;
    qDebug() << v3f4[0] << ", " << v3f4[1] << ", " << v3f4[2] << ")" << endl;
    qDebug() << "v1 -= v2, v1 = (";
    v3f1 -= v3f2;
    qDebug() << v3f1[0] << ", " << v3f1[1] << ", " << v3f1[2] << ")" << endl;
    qDebug() << endl;

    // Test * & /:
    qDebug() << "v1 * 0.4 = (";
    qDebug() << (v3f1 * 0.4)[0] << ", " << (v3f1 * 0.4)[1] << ", " << (v3f1 * 0.4)[2] << ")" << endl;
    qDebug() << "v1 / 0.4 = (";
    qDebug() << (v3f1 / 0.4)[0] << ", " << (v3f1 / 0.4)[1] << ", " << (v3f1 / 0.4)[2] << ")" << endl;
    qDebug() << "v2 *= 0.9, v2 = (";
    v3f2 *= 0.9;
    qDebug() << v3f2[0] << ", " << v3f2[1] << ", " << v3f2[2] << ")" << endl;
    qDebug() << "v2 /= 0.9, v2 = (";
    v3f2 /= 0.9;
    qDebug() << v3f2[0] << ", " << v3f2[1] << ", " << v3f2[2] << ")" << endl;
    qDebug() << endl;

    // Test normalize, dot product, cross product:
    qDebug() << "v1.normalize() = (";
    v3f1.normalize();
    qDebug() << v3f1[0] << ", " << v3f1[1] << ", " << v3f1[2] << ")" << endl;
    qDebug() << "v2.normalize() = (";
    v3f2.normalize();
    qDebug() << v3f2[0] << ", " << v3f2[1] << ", " << v3f2[2] << ")" << endl;
    qDebug() << "v1.dot(v2) = " << v3f1.dot(v3f2) << endl;
    qDebug() << "v1.cross(v2) = (";
    qDebug() << (v3f1.cross(v3f2))[0] << ", " << (v3f1.cross(v3f2))[1] << ", " << (v3f1.cross(v3f2))[2] << ")" << endl;
    qDebug() << endl;

    // Test == & !=:
    qDebug() << "v1 == v2 : " << (v3f1 == v3f2) << endl;
    qDebug() << "v1 != v2 : " << (v3f1 != v3f2) << endl;

    cin >> ignore;
    
    return 0;
} */


}