#include <stdlib.h>

#include "object.h"

void object_init()
{
	objects = malloc( MAX_OBJECTS * sizeof( struct object_type ) );
}

void object_update()
{
}

