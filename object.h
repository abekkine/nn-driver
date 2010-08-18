#ifndef OBJECT_H
#define OBJECT_H

#define MAX_OBJECTS 100

struct object_type {

	double x;
	double y;
	double r;
	unsigned int color;
};

struct object_type *objects;

void object_init();
void object_update();

#endif

