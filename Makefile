CC=g++
CFLAGS=-I

raytracer: raytracer.cpp objects.cpp 3dVector.cpp
	$(CC) -o raytracer raytracer.cpp objects.cpp 3dVector.cpp
