#include <stdlib.h>

#include "brain.h"
#include "display.h"
#include "motion.h"

float *motion_cells;
double motion_max_delta;

int _mouse_x_0;
int _mouse_y_0;
int _delta_x;
int _delta_y;

void motion_init()
{
	int i;

	_mouse_x_0 = display_mouse_x;
	_mouse_y_0 = display_mouse_y;

	motion_cells = malloc( sizeof( float ) * brain_num_motion );
	for( i=0; i<brain_num_motion; i++ )
	{
		motion_cells[i] = 0.0;
	}	
	
}

void motion_update()
{
	_delta_x = display_mouse_x - _mouse_x_0;
	_delta_y = display_mouse_y - _mouse_y_0;

	_mouse_x_0 = display_mouse_x;
	_mouse_y_0 = display_mouse_y;

	if ( _delta_y < -motion_max_delta ) _delta_y = -motion_max_delta;
	else if( _delta_y > motion_max_delta ) _delta_y = motion_max_delta;
	
	if( _delta_x < -motion_max_delta ) _delta_x = -motion_max_delta;
	else if( _delta_x > motion_max_delta ) _delta_x = motion_max_delta;

	if( _delta_x < 0 )
	{
		// < 0
		motion_cells[2] = -_delta_x / motion_max_delta;
		motion_cells[0] = 0.0;
	}
	else if ( _delta_x > 0 )
	{
		// > 0
		motion_cells[2] = 0.0;
		motion_cells[0] = _delta_x / motion_max_delta;
	}
	else
	{
		// = 0
		motion_cells[0] = 0.0;
		motion_cells[2] = 0.0;
	}

	if( _delta_y < 0 )
	{
		// < 0
		motion_cells[3] = -_delta_y / motion_max_delta;
		motion_cells[1] = 0.0;
	}
	else if ( _delta_y > 0 )
	{
		// > 0
		motion_cells[1] = _delta_y / motion_max_delta;
		motion_cells[3] = 0.0;
	}
	else
	{
		// = 0
		motion_cells[1] = 0.0;
		motion_cells[3] = 0.0;
	}
}

