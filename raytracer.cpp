#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include "objects.h"
using namespace std;
// add definitions for compiling on Windows
#if defined __linux__ || defined __APPLE__
#else
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

// ray tracer
#define MAX_RAY_DEPTH 5
float mix(const float &a, const float &b, const float &mix){
	return b * mix + a * (1- mix);
}
// main tracer
Vector3 trace(
	const Ray& ray,
	const std::vector<Sphere> &spheres,
	const int &depth)
{
	float tnear = INFINITY;
	const Sphere* sphere = NULL;
	// find closest intersection of ray and spheres in the scene
	for (unsigned i = 0; i < spheres.size(); ++i){
		float t0 = INFINITY, t1 = INFINITY;
		if (spheres[i].intersect(ray, t0, t1)){
			if (t0 < 0) t0 = t1;
			if (t0 < tnear) {
				tnear = t0;
				sphere = &spheres[i];
			}
		}
	}
	// return black background if no intersection
	if (!sphere) return Vector3(2);
	// else proceed with color calculation for raytracing
	bool inside = false;
	Vector3 raydir = ray.direction();
	Vector3 rayori = ray.origin();
	Vector3 surfacecol = 0;
	Vector3 intersectPos = rayori + raydir * tnear; // intersecton point
	Vector3 intersectNormal = intersectPos - sphere->center; // normal at intersection point

	intersectNormal.normalize();
	float bias = 1e-4;

	// if normal and the view direction aren't opposite, flip the normal direction. 
	// This means we've inside the sphere.
	if (raydir.dot(intersectNormal) > 0){
		intersectNormal = -intersectNormal;
		inside = true;
	}
	// if transparency > 0 or reflection > 0, then means transparent/reflective object, 
	// need to recursively raytrace to calc those components.
	if ((sphere->transp > 0 || sphere->refl > 0) && depth < MAX_RAY_DEPTH){
		float facingratio = -raydir.dot(intersectNormal);

		float fresneleffect = mix(pow(1-facingratio, 3), 1, 0.1);
		// compute reflection direction and cast a ray to do further raytracing
		Vector3 refldir = raydir - intersectNormal * 2 * raydir.dot(intersectNormal);
		refldir.normalize();
		Vector3 reflori = intersectPos + intersectNormal * bias;
		Vector3 reflection = trace(Ray(reflori, refldir), spheres, depth + 1);
		Vector3 refraction = 0;
		// compute refraction if sphere is also transparent
		if (sphere->transp){
			float ior = 1.1, eta;
			if (inside) eta = ior;
			else{
				eta = 1/ior;
			}
			float cosi = -intersectNormal.dot(raydir);
			float k = 1 - eta * eta * (1 - cosi * cosi);
			Vector3 refrdir = raydir * eta + intersectNormal * (eta * cosi - sqrt(k));
			Vector3 refrori = intersectPos - intersectNormal * bias;
			refrdir.normalize();
			refraction = trace(Ray(refrori, refrdir), spheres, depth + 1);
		}
		// surface color becomes a mix of reflected and refracted components
		surfacecol = (reflection * fresneleffect + refraction * (1- fresneleffect) * sphere->transp) * sphere->surfaceCol;
	}
	else {
		// diffuse object, no need to cast more rays
		for (unsigned i = 0; i < spheres.size(); ++i){
			if (spheres[i].emissionCol.x > 0){
				// light source
				Vector3 transmission = 1;
				Vector3 lightdir = spheres[i].center - intersectPos;
				lightdir.normalize();
				for (unsigned j = 0; j < spheres.size(); ++j){
					if (i != j) {
						float t0, t1;
						// something is blocking the light source, so shadowed
						if (spheres[j].intersect(Ray(intersectPos + intersectNormal * bias, lightdir), t0, t1)){
							transmission = 0;
							break;
						}
					}
				}
				surfacecol += sphere->surfaceCol * spheres[i].emissionCol * transmission * std::max(float(0), intersectNormal.dot(lightdir));
			}
		}
	}
	return surfacecol + sphere->emissionCol;
}

/* rendering function for the scene. defines the size of the image, compute a camera ray for
   each pixel of the image, trace it and returns a color. once the ray tracing is done, 
   convert the image onto a ppm and display it.
*/

void render(const std::vector<Sphere> &spheres){
	// set camera
	unsigned width = 1280, height = 640;
	Vector3 *image = new Vector3[width * height],  *pixel = image;
	float invWidth = 1/ float(width), invHeight = 1/ float(height);
	float fov = 30;
	float asp = width / float(height);
	float angle = tan(M_PI * 0.5 * fov/180.);
	// trace rays
	for (unsigned y=0; y<height; ++y){
		for (unsigned x=0; x<width; ++x, ++pixel){
			Vector3 rayori = Vector3(0, 0, 0);
			float xval = (2 * ((x+0.5)* invWidth) - 1) * angle * asp;
			float yval = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			float zval = -1;
			Vector3 raydir = Vector3(xval, yval, zval).normalize();
			*pixel = trace(Ray(rayori, raydir), spheres, 0);
		}
	}

	// save result to .ppm
	std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < width * height; ++i){
		ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) 
		    << (unsigned char)(std::min(float(1), image[i].y) * 255)
		    << (unsigned char)(std::min(float(1), image[i].z) * 255);
	}
	ofs.close();
	delete [] image;
}

int main(int argc, char **argv) {
	srand48(13);
	std::vector<Sphere> spheres;
	// position, radius, surface color, reflectivity, transparency, emission color
	spheres.push_back(Sphere(Vector3(0.0, -10004, -300), 10000, Vector3(0.20, 0.20, 0.20), 0, 0.0));
    spheres.push_back(Sphere(Vector3(0.0, 25, -200), 20, Vector3(1.00, 0.32, 0.36), 1, 0.5));
    spheres.push_back(Sphere(Vector3(5.0, -2, -18), 4, Vector3(0.90, 0.76, 0.46), 1, 0.0));
    spheres.push_back(Sphere(Vector3(5.0, 0, -25), 3, Vector3(0.65, 0.77, 0.97), 1, 0.0));
    spheres.push_back(Sphere(Vector3(-5.5, 0, -15), 3, Vector3(1, 1, 1), 1, 0.0));
    // light
    spheres.push_back(Sphere(Vector3(0.0, 20, -30), 3, Vector3(0.00, 0.00, 0.00), 0, 0.0, Vector3(2)));
    render(spheres); 
	return 0;
}