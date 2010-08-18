#include "environment.h"
#include "object.h"

double environment_visibility;
double environment_radius;

void environment_init()
{
	object_init();
}

void environment_update()
{
	object_update();
}

