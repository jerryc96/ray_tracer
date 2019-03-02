#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include "objects.h"
using namespace std;

// basic 3D vector
Vector3::Vector3(){
	x = 0, y = 0, z = 0;
}
Vector3::Vector3(float xx){
	x = xx, y = xx, z = xx;
}
Vector3::Vector3(float xx, float yy, float zz){
	x = xx, y = yy, z = zz;
}

Ray::Ray(const Vector3 &a, const Vector3&b){
	ori = a, dir = b;
}
Sphere::Sphere(		
	const Vector3 &c,
	const float &r,
	const Vector3 &sc,
	const float &ref,
	const float &trans,
	const Vector3 &ec)
{
	center = c, radius = r, radius2 = r*r;
	surfaceCol = sc, emissionCol = ec, transp = trans, refl = ref;
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

// Ray functions
Vector3 Ray::origin() const {
	return ori;
}
Vector3 Ray::direction() const {
	return dir;
}
Vector3 Ray::point_at_parameter(float t) const {
	return ori + dir * t;
}
// Sphere functions
// calculate intersection btn ray and sphere
// three cases for line-sphere intersection:
// 1. no intersection, return false,
// 2. one point intersection (aka tangent)
// 3. two point intersection (there are 3 subcases)
//    case A: t0 and t1 are positive, which means ray is facing the sphere and intersecting
//    case B: one of them is positive and the other negative, meaning ray is shooting from the inside
//    case C: both of them are negative, which means the ray is pointing in the wrong direction,
//            so intersection's actually impossible
bool Sphere::intersect(const Ray &r, float &t0, float &t1) const {
	Vector3 l = center - r.origin();
	Vector3 raydir = r.direction();
	float tca = l.dot(raydir);
	if (tca < 0) return false;
	float d2 = l.dot(l) - tca * tca;
	if (d2 > radius2) return false;
	float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;
	return true;
}
