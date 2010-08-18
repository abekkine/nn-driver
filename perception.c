#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "display.h"
#include "environment.h"
#include "brain.h"
#include "perception.h"

float *perception_retina;

void perception_init()
{
	perception_retina = malloc( 3 * sizeof( float ) * brain_num_visual );
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

	for( i=0; i<brain_num_visual; i++ )
	{
		search_angle = 2.0 * i * M_PI / brain_num_visual;
		perception_retina[3*i+0] = 0.3;
		perception_retina[3*i+1] = 0.3;
		perception_retina[3*i+2] = 0.3;
		for( search_range = 4.0 * self->r; search_range < environment_visibility; search_range += 10.0 )
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

