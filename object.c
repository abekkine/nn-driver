#include <stdlib.h>

#include "object.h"

struct object_type *object_list;
struct object_type *self;

int object_num;

void object_init()
{
	object_list = malloc( MAX_OBJECTS * sizeof( struct object_type ) );
	// First object in list is self.
	self = &object_list[0];
}

void object_update()
{
}

