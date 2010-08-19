#include <stdlib.h>

#include "object.h"
#include "display.h"
#include "brain.h"
#include "motion.h"

int motion_num_motion;
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

	motion_cells = malloc( sizeof( float ) * motion_num_motion );
	for( i=0; i<motion_num_motion; i++ )
	{
		motion_cells[i] = 0.0;
	}	
	
}

void motion_update()
{
	if( brain_in_training_mode() )
	{
		motion_compute_cell_values();
	}
	else if( brain_in_running_mode() )
	{
		motion_use_cell_values();
	}
	else
	{
		self->x = display_mouse_x;
		self->y = display_mouse_y;
	}
}

void motion_use_cell_values()
{
	int i;
	_delta_x = (motion_cells[0] - motion_cells[2]);
	_delta_y = (motion_cells[1] - motion_cells[3]);

	self->x += _delta_x;
	self->y += _delta_y;
}

void motion_compute_cell_values()
{
	// Update object by mouse.
	self->x = display_mouse_x;
	self->y = display_mouse_y;

	// Update coordinate diffences.
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

