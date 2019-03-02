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

	Vector3 operator * (const float &f) const {
		return Vector3(x * f, y * f, z * f);
	}
	Vector3 operator * (const Vector3 &v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	float dot(const Vector3 &v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	Vector3 operator + (const Vector3 &v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator - (const Vector3 &v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator - () const {
		return Vector3(-x, -y, -z);
	}
	// if change self, don't have const.
	Vector3 & operator += (const Vector3 &v) {
		x += v.x, y += v.y, z += v.z;
		return *this;
	}
	Vector3 & operator *= (const Vector3 &v) {
		x *= v.x, y *= v.y, z *= v.z;
		return *this;
	}
	// 2norm, normalization, etc.
	float length2() const {
		return x*x + y*y + z*z;
	}
	float length() const {
		return sqrt(length2());
	}
	Vector3 & normalize() {
		float nor2 = length2();
		if (nor2 > 0) {
			float inv = 1 / sqrt(nor2);
			x *= inv, y *= inv, z *= inv;
		}
		return *this;
	}

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
	Vector3 origin() const {
		return ori;
	}
	Vector3 direction() const {
		return dir;
	}
	Vector3 point_at_parameter(float t) const {
		return ori + dir * t;
	}
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

	// assume 
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