#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include "objects.h"
using namespace std;
#define EPSI 1.00001

Ray::Ray(const Vector3 &a, const Vector3&b){
	ori = a, dir = b;
	// store inverse of ray direction and signs for ray-box intersection
	inv_dir = Vector3(1/b.xval(), 1/b.yval(), 1/b.zval());
	sign[0] = (inv_dir.xval() < 0);
	sign[1] = (inv_dir.yval() < 0);
	sign[2] = (inv_dir.zval() < 0);
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

Box::Box(
	const Vector3 &ori, 
	const Vector3 &end,
	const Vector3 &sc,
	const float &ref,
	const float &trans,
	const Vector3 &ec)
{
	bounds[0] = ori, bounds[1] = end;
	surfaceCol = sc, emissionCol = ec, transp = trans, refl = ref;
}

Plane::Plane(
	const Vector3 &norm,
	const Vector3 &pt,
	const Vector3 &sc,
	const float &ref,
	const float &trans,
	const Vector3 &ec)
{
	normal = norm, point = pt;
	surfaceCol = sc, emissionCol = ec, transp = trans, refl = ref;
}

// Ray functions
Vector3 Ray::origin() const {
	return ori;
}
Vector3 Ray::direction() const {
	return dir;
}
Vector3 Ray::inv_direction() const {
	return inv_dir;
}
Vector3 Ray::point_at_parameter(float t) const {
	return ori + dir * t;
}

std::ostream & operator << (std::ostream &os, const Ray &r){
	os << "Ray origin: " << r.origin() << "\n";
	os << "Ray direction: " << r.direction() << "\n";
	return os;
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
Vector3 Sphere::get_center() const {
	return center;
}
Vector3 Sphere::get_radius() const {
	return radius;
}

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

Vector3 Sphere::nhit(Vector3 Phit) const{
	return Phit - center;
}

std::ostream & operator << (std::ostream &os, const Sphere &s){
	os << "Origin, Radius: " << s.get_center() << ", " << s.get_radius() << "\n";
	return os;
}

// box functions
Vector3 Box::get_origin() const {
	return bounds[0];
}
Vector3 Box::get_dimensions() const {
	return bounds[1] - bounds[0];
}
/*
Calculate intersection for box
*/
bool Box::intersect(const Ray &r, float &t0, float &t1) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = (bounds[r.sign[0]].xval() - r.ori.xval()) * r.inv_dir.xval();
	tmax = (bounds[1 - r.sign[0]].xval() - r.ori.xval()) * r.inv_dir.xval();
	tymin = (bounds[r.sign[1]].yval() - r.ori.yval()) * r.inv_dir.yval();
	tymax = (bounds[1 - r.sign[1]].yval() - r.ori.yval()) * r.inv_dir.yval();

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (bounds[r.sign[2]].zval() - r.ori.zval()) * r.inv_dir.zval();
	tzmax = (bounds[1 - r.sign[2]].zval() - r.ori.zval()) * r.inv_dir.zval();
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	float prevt0;
	prevt0 = t0;
	t0 = tmin;
	if (t0 < 0){
		t0 = tmax;
		if (t0 < 0) {
			t0 = prevt0;
			return false;
		}
	}
    return true;
}
// Calculate intersection normal with the box
Vector3 Box::nhit(Vector3 Phit) const {
	Vector3 intersectNormal, c, p, d;
	c = (bounds[0] + bounds[1]) * 0.5;
	p = Phit - c;
	d = (bounds[0] - bounds[1]) * 0.5;
	float bias = EPSI;
	intersectNormal = Vector3(float(int(p.xval()/abs(d.xval()) * bias)),
							  float(int(p.yval()/abs(d.yval()) * bias)),
							  float(int(p.zval()/abs(d.zval()) * bias))
							 );
	return intersectNormal;
}

std::ostream & operator << (std::ostream &os, const Box &b){
	os << "Origin, Dimensions: " << b.get_origin() << ", " << b.get_dimensions() << "\n";
	return os;
}

bool Plane::intersect(const Ray &r, float &t0, float &t1) const {
	float numer, denom, t;
	numer = normal.dot(point - r.origin());
	denom = normal.dot(r.direction());
	if (denom == 0) return false;
	t = numer/denom;
	if (t < 0) return false;
	else {
		t0 = t;
		return true;
	}
}

Vector3 Plane::nhit(Vector3 Phit) const {
	return normal;
}