#include <iostream>
class Vector3;
class Ray;

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
	Vector3 & operator /= (const float &r);
	// 2norm, normalization, etc.
	float length2() const;
	float length() const;

	float xval() const;
	float yval() const;
	float zval() const;
	Vector3 & normalize();
};
std::ostream & operator << (std::ostream &os, const Vector3 &v);