#include <stdlib.h>
#include <fann.h>

#include "perception.h"
#include "motion.h"
#include "brain.h"

int brain_num_layers;
int brain_num_hidden;

int _num_input;
int _num_output;
struct fann *_ann;
fann_type *_output_array;
int _training_mode;
int _running_mode;

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

	_training_mode = 1;
	_running_mode = 0;
}

int brain_in_running_mode()
{
	return _running_mode;
}

int brain_in_training_mode()
{
	return _training_mode;
}

void brain_update()
{
	if( _training_mode )
	{
		brain_train();
	}
	else if( _running_mode )
	{
		brain_run();
	}
}

void brain_set_mode( int mode )
{
	switch( mode )
	{
		case BRAIN_TRAINING_MODE:
			_training_mode = 1;
			_running_mode = 0;
			break;

		case BRAIN_RUNNING_MODE:
			_training_mode = 0;
			_running_mode = 1;
			break;

		case BRAIN_FREE_MODE:
			_training_mode = 0;
			_running_mode = 0;
			break;
	}
}

void brain_train()
{
	fann_train( _ann, (fann_type *) perception_retina, (fann_type *) motion_cells );
}

void brain_run()
{
	int i;
	_output_array = fann_run( _ann, (fann_type *) perception_retina );

	for( i=0; i<_num_output; i++ )
	{
		motion_cells[i] = (float) _output_array[i];
	}

}

