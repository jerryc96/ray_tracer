#include <iostream>

class Vector3 {
public:
	float x, y, z;
	// default vector, 0 in all directions
	Vector3();
	// 1 input, assume equal distance in all directions
	Vector3(float xx);
	Vector3(float xx, float yy, float zz);
	// vector operators

	Vector3 operator * (const float &f) const;
	Vector3 operator * (const Vector3 &v) const;
	float dot(const Vector3 &v) const;
	Vector3 operator + (const Vector3 &v) const;
	Vector3 operator - (const Vector3 &v) const;
	Vector3 operator - () const;
	// if change self, don't have const.
	Vector3 & operator += (const Vector3 &v);
	Vector3 & operator *= (const Vector3 &v);
	// 2norm, normalization, etc.
	float length2() const;
	float length() const;
	Vector3 & normalize();

	friend std::ostream & operator << (std::ostream &os, const Vector3 &v){
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};
// The ray, contains the ray's original position and a direction
class Ray {
public:
	Vector3 ori, dir;
	Ray(const Vector3 &a, const Vector3 &b);
	Vector3 origin() const;
	Vector3 direction() const;
	Vector3 point_at_parameter(float t) const;
};
// basic object template
class Object {
public:
	Vector3 surfaceCol, emissionCol;
	virtual bool intersect(const Ray &r, float &t0, float &t1) const = 0;
	virtual ~Object(){};
	Object(){};
};


// basic sphere
class Sphere : public Object {
public:
	Vector3 center; //init position
	Vector3 surfaceCol, emissionCol; //surface colour and emission for ray tracing calcs
	float radius, radius2; // radius and squared radius
	float transp, refl; // transparency and reflective properties

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
	bool intersect(const Ray &r, float &t0, float &t1) const;
};

// add cylinders and squares
// class Box : public Object{
// 	Vector3 origin;
// 	Vector3 bound;
// 	// create cube with center at origin (0, 0, 0) and and side length s 
// 	Box(float &s);
// 	// rectanglular prism with center at origin and dimensions v
// 	Box(Vector3 &v);
// 	// rectangular prism
// 	Box(Vector3 &ori, Vector3 &end);
// }