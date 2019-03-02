CC=g++
CFLAGS=-I

raytracer: raytracer.cpp objects.cpp
	$(CC) -o raytracer raytracer.cpp objects.cpp
