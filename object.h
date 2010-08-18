#ifndef OBJECT_H
#define OBJECT_H

#define MAX_OBJECTS 100

struct object_type {

	double x;
	double y;
	double r;
	unsigned int color;
};

void object_init();
void object_update();

extern struct object_type *object_list;
extern struct object_type *self;
extern int object_num;

#endif

