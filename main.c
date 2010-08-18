#include <stdio.h>

#include "configuration.h"
#include "display.h"
#include "environment.h"
#include "perception.h"
#include "brain.h"
#include "motion.h"
#include "object.h"

int main( int argc, char *argv[] )
{
	int application_quit = 0;

	puts( "NNDriver Application Begin" );

	configuration_init();

	object_init();
	configuration_read();

	configuration_dump();

	environment_init();
	perception_init();
	brain_init();
	motion_init();

	display_init();

	while( application_quit == 0 )
	{
		environment_update();
		perception_update();
		brain_update();
		motion_update();

		display_update();
		display_poll_events();

		application_quit = display_check_quit();
	}

	puts( "NNDriver Application End" );

	return 1;
}
