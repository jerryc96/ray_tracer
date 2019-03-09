#include <iostream>
#include "3dVector.h"

class Ray;
class Object;
class Sphere;
class Box;

// The ray, contains the ray's original position and a direction
class Ray {
public:
	int sign[3];
	Vector3 ori, dir, inv_dir;
	Ray(const Vector3 &a, const Vector3 &b);
	Vector3 origin() const;
	Vector3 direction() const;
	Vector3 inv_direction() const;
	Vector3 point_at_parameter(float t) const;
};
std::ostream & operator << (std::ostream &os, const Ray &r);
// basic object template
class Object {
public:
	Vector3 surfaceCol, emissionCol; //surface colour and emission for ray tracing calcs
	float transp, refl; // transparency and reflective properties
	virtual bool intersect(const Ray &r, float &t0, float &t1) const = 0;
	virtual Vector3 nhit (Vector3 Phit) const = 0;
	virtual ~Object(){};
	Object(){};
};

// basic sphere
class Sphere : public Object {
public:
	Vector3 center; //init position
	float radius, radius2; // radius and squared radius

	Sphere(
		const Vector3 &c,
		const float &r,
		const Vector3 &sc,
		const float &ref = 0,
		const float &trans = 0,
		const Vector3 &ec = 0);
	/*
	    calculate intersection btn ray and sphere
	    three cases for line-sphere intersection:
	    1. no intersection, return false,
	    2. one point intersection (aka tangent)
	    3. two point intersection (there are 3 subcases)
	       case A: t0 and t1 are positive, which means ray is facing the sphere and intersecting
	       case B: one of them is positive and the other negative, meaning ray is shooting from the inside
	       case C: both of them are negative, which means the ray is pointing in the wrong direction,
	               so intersection's actually impossible
	*/
	Vector3 get_center() const;
	Vector3 get_radius() const;
	bool intersect(const Ray &r, float &t0, float &t1) const;
	Vector3 nhit (Vector3 Phit) const;
};
std::ostream & operator << (std::ostream &os, const Sphere &s);

// Simple 3D box. Assume the box is axis aligned.
class Box : public Object{
public:
	Vector3 bounds[2];
	// create box that starts at ori and and ends at bound
	Box(const Vector3 &ori, 
		const Vector3 &end,
		const Vector3 &sc,
		const float &ref = 0,
		const float &trans = 0,
		const Vector3 &ec = 0);

	Vector3 get_origin() const;
	Vector3 get_dimensions() const;
	bool intersect(const Ray &r, float &t0, float &t1) const;
	Vector3 nhit (Vector3 Phit) const;
};
std::ostream & operator << (std::ostream &os, const Box &b);