#include <string.h>
#include <stdlib.h>
#include <fann.h>

#include "display.h"
#include "perception.h"
#include "motion.h"
#include "brain.h"

int brain_num_layers;
int brain_num_hidden;
int brain_display_mode_enable;

int _brain_mode;
int _num_input;
int _num_output;
struct fann *_ann;
fann_type *_output_array;

char _info_brain_mode_str[256];
char *_brain_mode_str_list[3]= { "Training Mode", "Running Mode", "Free Mode" };

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

	_ann = fann_create_standard_array( brain_num_layers, layers );

	fann_set_activation_function_hidden( _ann, FANN_SIGMOID_SYMMETRIC );
	fann_set_activation_function_output( _ann, FANN_SIGMOID_SYMMETRIC );
	fann_set_training_algorithm( _ann, FANN_TRAIN_INCREMENTAL );
	fann_randomize_weights( _ann, 0.0, 1.0 );

	brain_display_mode_enable = 1;
	strcpy( _info_brain_mode_str, "Free Mode" );
	display_add_info( 250, 20, _info_brain_mode_str, &brain_display_mode_enable );

	brain_set_mode( BRAIN_FREE_MODE );
}

void brain_update()
{
	switch( _brain_mode )
	{
		case BRAIN_RUNNING_MODE:
			brain_run();
			break;

		case BRAIN_TRAINING_MODE:
			brain_train();
			break;

		case BRAIN_FREE_MODE:
		default:
			break;
	}
}

int brain_get_mode()
{
	return _brain_mode;
}

void brain_set_mode( int mode )
{
	_brain_mode = mode;
	strcpy( _info_brain_mode_str, _brain_mode_str_list[mode] );	
	motion_reset_cell_values();
}

void brain_train()
{
	fann_train( _ann, (fann_type *) perception_retina, (fann_type *) motion_cells );
}

void brain_run()
{
	int i;

	motion_reset_cell_values();

	_output_array = fann_run( _ann, (fann_type *) perception_retina );

	for( i=0; i<_num_output; i++ )
	{
		motion_cells[i] = (float) _output_array[i];
	}
}

