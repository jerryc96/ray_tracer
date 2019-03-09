#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include "3dVector.h"
using namespace std;

// vector, ray, object constructors
Vector3::Vector3(){
	x = 0, y = 0, z = 0;
}
Vector3::Vector3(float xx){
	x = xx, y = xx, z = xx;
}
Vector3::Vector3(float xx, float yy, float zz){
	x = xx, y = yy, z = zz;
}

// Vector3 functions and operators
Vector3 Vector3::operator * (const float &f) const {
	return Vector3(x * f, y * f, z * f);
}
Vector3 Vector3::operator * (const Vector3 &v) const {
	return Vector3(x * v.x, y * v.y, z * v.z);
}
float Vector3::dot(const Vector3 &v) const {
	return x * v.x + y * v.y + z * v.z;
}
Vector3 Vector3::operator + (const Vector3 &v) const {
	return Vector3(x + v.x, y + v.y, z + v.z);
}
Vector3 Vector3::operator - (const Vector3 &v) const {
	return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3 Vector3::operator - () const {
	return Vector3(-x, -y, -z);
}
// if change self, don't have const.
Vector3 & Vector3::operator += (const Vector3 &v) {
	x += v.x, y += v.y, z += v.z;
	return *this;
}
Vector3 & Vector3::operator *= (const Vector3 &v) {
	x *= v.x, y *= v.y, z *= v.z;
	return *this;
}
Vector3 & Vector3::operator /= (const float &f) {
	x /= f, y /= f, z/= f;
	return *this;
}
// 2norm, normalization, etc.
float Vector3::length2() const {
	return x*x + y*y + z*z;
}
float Vector3::length() const {
	return sqrt(length2());
}
Vector3 & Vector3::normalize() {
	float nor2 = length2();
	if (nor2 > 0) {
		float inv = 1 / sqrt(nor2);
		x *= inv, y *= inv, z *= inv;
	}
	return *this;
}
float Vector3::xval() const {
	return x;
}
float Vector3::yval() const {
	return y;
}
float Vector3::zval() const {
	return z;
}
// print statement for vector
std::ostream & operator << (std::ostream &os, const Vector3 &v){
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
}