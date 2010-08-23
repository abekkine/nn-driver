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
int brain_display_save_file;

int _brain_mode;
int _num_input;
int _num_output;
struct fann *_ann;
fann_type *_output_array;
int _brain_num_connections;
struct fann_connection *_brain_connections;

char _info_brain_mode_str[256];
char *_brain_mode_str_list[3]= { "Training Mode", "Running Mode", "Free Mode" };
char _brain_save_file[] = "brain_float_000.net";
int _brain_save_file_index = 0;

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
	//fann_randomize_weights( _ann, 0.0, 1.0 );

	// Allocate connections array.
	_brain_num_connections = fann_get_total_connections( _ann );
	_brain_connections = malloc( sizeof( struct fann_connection ) * _brain_num_connections );
	//DEBUG
	printf( "_brain_num_connections(%d)\n", _brain_num_connections );
	//END

	// Brain save file name
	sprintf( _brain_save_file, "brain_float_%03d.net", _brain_save_file_index );
	brain_display_save_file = 1;
	display_add_info( 10, 472, _brain_save_file, &brain_display_save_file );

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

	//DEBUG
	brain_get_connections();
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

void brain_get_connections()
{
	int i;
	struct fann_connection min_weight;
	struct fann_connection max_weight;

	fann_get_connection_array( _ann, _brain_connections );

	//DEBUG
	min_weight.weight = 1000000000.0;
	max_weight.weight = -1000000000.0;
	for( i=0; i<_brain_num_connections; i++ )
	{
		if( _brain_connections[i].weight < min_weight.weight )
		{
			min_weight = _brain_connections[i];
		}
		else if( _brain_connections[i].weight > max_weight.weight )
		{
			max_weight = _brain_connections[i];
		}	
	}

	printf( "MIN MAX\n" );
	printf( "%lf %lf\n", min_weight.weight, max_weight.weight );
	printf( "%d %d\n", min_weight.from_neuron, max_weight.from_neuron );
	printf( "%d %d\n", min_weight.to_neuron, max_weight.to_neuron );
}

void brain_next_save_file()
{
	_brain_save_file_index++;
	_brain_save_file_index %= 100;

	sprintf( _brain_save_file, "brain_float_%03d.net", _brain_save_file_index );
}

void brain_prev_save_file()
{
	_brain_save_file_index--;
	if( _brain_save_file_index < 0 )
		_brain_save_file_index = 99;

	sprintf( _brain_save_file, "brain_float_%03d.net", _brain_save_file_index );
}

void brain_save()
{
	fann_save( _ann, _brain_save_file );
}

void brain_load()
{
	fann_destroy( _ann );

	_ann = fann_create_from_file( _brain_save_file );
}
