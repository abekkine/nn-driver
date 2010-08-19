#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "display.h"
#include "environment.h"
#include "perception.h"

float *perception_retina;
int perception_num_vision;
double perception_range_start;
double perception_range_end;
double perception_scan_step;

double _search_range_start;
double _search_range_end;
double _search_range_step;

void perception_init()
{
	perception_retina = malloc( 3 * sizeof( float ) * perception_num_vision );

	_search_range_start = perception_range_start * self->r;
	if( perception_range_end < perception_range_start )
	{
		_search_range_end = environment_visibility;
	}
	else
	{
		_search_range_end = perception_range_end * self->r;
	}
	_search_range_step = perception_scan_step;
}

void perception_update()
{
}

void perception_loop()
{
	int i;
	double search_range; 
	double search_angle;
	double search_x, search_y;
	unsigned int pixel_buffer;
	float red, green, blue;

	for( i=0; i<perception_num_vision; i++ )
	{
		search_angle = 2.0 * i * M_PI / perception_num_vision;
		perception_retina[3*i+0] = 0.3;
		perception_retina[3*i+1] = 0.3;
		perception_retina[3*i+2] = 0.3;
		for( search_range = _search_range_start; search_range < _search_range_end; search_range += _search_range_step )
		{
			search_x = self->x + search_range * cos( search_angle );
			search_y = self->y + search_range * sin( search_angle );

			pixel_buffer = display_check_pixel( search_x, search_y );
			if( pixel_buffer != 0 )
			{
				// Search is over.
				display_get_pixel_color( search_x, search_y, &red, &green, &blue );
			
				perception_retina[3*i+0] = red;
				perception_retina[3*i+1] = green;
				perception_retina[3*i+2] = blue;
				break;
			}
		}
	}
}

