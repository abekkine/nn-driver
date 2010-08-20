#include <stdlib.h>

#include "object.h"
#include "display.h"
#include "brain.h"
#include "motion.h"

int motion_num_motion;
float *motion_cells;
double motion_max_delta;
int motion_mouse_move_enable;

int _mouse_x_0;
int _mouse_y_0;

void motion_init()
{
	int i;

	motion_mouse_move_enable = 0;
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
	int b_mode = brain_get_mode();

	switch( b_mode )
	{
		case BRAIN_TRAINING_MODE:
			if( motion_mouse_move_enable )
				motion_compute_cell_values();
			break;

		case BRAIN_RUNNING_MODE:
			motion_use_cell_values();
			break;

		case BRAIN_FREE_MODE:
		default:
			break;
	}

	if( b_mode != BRAIN_RUNNING_MODE )
	{
		if( motion_mouse_move_enable )
		{
			self->x = display_mouse_x;
			self->y = display_mouse_y;
		}
	}
}

void motion_use_cell_values()
{
	self->x -= motion_cells[0];
	self->x += motion_cells[2];

	self->y -= motion_cells[1];
	self->y += motion_cells[3];
}

void motion_compute_cell_values()
{
	int delta_x, delta_y;

	motion_reset_cell_values();

	// Update object by mouse.
	if( motion_mouse_move_enable )
	{
		self->x = display_mouse_x;
		self->y = display_mouse_y;
	}

	// Update coordinate diffences.
	delta_x = display_mouse_x - _mouse_x_0;
	delta_y = display_mouse_y - _mouse_y_0;

	_mouse_x_0 = display_mouse_x;
	_mouse_y_0 = display_mouse_y;

	motion_cells[0] = 0.0;
	motion_cells[1] = 0.0;
	motion_cells[2] = 0.0;
	motion_cells[3] = 0.0;
	if( delta_x < 0 )
	{
		// < 0
		motion_cells[0] = 1.0;
	}
	else if ( delta_x > 0 )
	{
		// > 0
		motion_cells[2] = 1.0;
	}

	if( delta_y < 0 )
	{
		// < 0
		motion_cells[1] = 1.0;
	}
	else if ( delta_y > 0 )
	{
		// > 0
		motion_cells[3] = 1.0;
	}
}

void motion_reset_cell_values()
{
	int i;

	if( motion_cells != NULL )
	{
		for( i=0; i<motion_num_motion; i++ )
			motion_cells[i] = 0.0;
	}
}
