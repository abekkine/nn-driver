#include <stdlib.h>

#include "perception.h"
#include "motion.h"
#include "brain.h"

int brain_num_layers;
int brain_num_hidden;

int _num_input;
int _num_output;

void brain_init()
{
	int i;
	unsigned int *layers;

	layers = malloc( brain_num_layers * sizeof( unsigned int ) );

	_num_input = perception_num_vision * 3;
	_num_output = motion_num_motion;

	for( i=0; i<brain_num_layers; i++ )
	{
		layers[i] = brain_num_hidden;
	}
	layers[0] = _num_input;
	layers[brain_num_layers-1] = _num_output;
}

void brain_update()
{
}

