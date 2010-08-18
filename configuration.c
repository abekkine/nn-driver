#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "definitions.h"
#include "object.h"
#include "display.h"
#include "brain.h"
#include "environment.h"
#include "configuration.h"

static config_t config;
static config_setting_t *setting_cfg;
static FILE *f_config;

void configuration_dump()
{
	int i;

	printf( "screen(%d,%d)\n", display_screen_width, display_screen_height );
	printf( "display_bg_color(%08x)\n", display_bg_color );
	printf( "brain_num_visual(%d)\n", brain_num_visual );
	printf( "brain_num_hidden(%d)\n", brain_num_hidden );
	printf( "brain_num_motion(%d)\n", brain_num_motion );
	printf( "environment_visibility(%lf)\n", environment_visibility );
	printf( "environment_radius(%lf)\n", environment_radius );
	printf( "self->x(%lf)\n", self->x );
	printf( "self->y(%lf)\n", self->y );
	printf( "self->r(%lf)\n", self->r );
	printf( "self->color(%08x)\n", self->color );

	for( i=0; i<object_num; i++ )
	{
		printf( "obj[%d].x(%lf)\n", i, object_list[i].x );
		printf( "obj[%d].y(%lf)\n", i, object_list[i].y );
		printf( "obj[%d].r(%lf)\n", i, object_list[i].r );
		printf( "obj[%d].color(%08x)\n", i, object_list[i].color );
	}
}

void configuration_init()
{
	config_init( &config );
}

void configuration_read()
{
	if( config_read_file( &config, "nndriver.conf" ) )
	{
		setting_cfg = config_lookup( &config, "nndriver.display" );
		if( setting_cfg != NULL )
		{
			config_setting_t *screen_cfg = config_lookup( &config, "nndriver.display.screen" );
			if( screen_cfg != NULL )
			{
				config_setting_lookup_int( screen_cfg, "width", &display_screen_width );
				config_setting_lookup_int( screen_cfg, "height", &display_screen_height );
			}

			config_setting_lookup_int( setting_cfg, "background", &display_bg_color );
		}

		setting_cfg = config_lookup( &config, "nndriver.brain" );
		if( setting_cfg != NULL )
		{
			config_setting_lookup_int( setting_cfg, "num_visual", &brain_num_visual );
			config_setting_lookup_int( setting_cfg, "num_hidden", &brain_num_hidden );
			config_setting_lookup_int( setting_cfg, "num_motion", &brain_num_motion );
		}

		setting_cfg = config_lookup( &config, "nndriver.environment" );
		if( setting_cfg != NULL )
		{
			config_setting_lookup_float( setting_cfg, "visibility", &environment_visibility );

			config_setting_lookup_float( setting_cfg, "radius", &environment_radius );

			config_setting_t *self_cfg = config_lookup( &config,  "nndriver.environment.self" );
			if( self_cfg != NULL )
			{
				config_setting_lookup_float( self_cfg, "X", &(self->x) );
				config_setting_lookup_float( self_cfg, "Y", &(self->y) );
				config_setting_lookup_float( self_cfg, "R", &(self->r) );
				config_setting_lookup_int( self_cfg, "color", &(self->color) );
			}

			config_setting_t *objects_cfg = config_lookup( &config, "nndriver.environment.objects" );
			if( objects_cfg != NULL )
			{
				int i;
				object_num = config_setting_length( objects_cfg );

				for( i=0; i<object_num && (i+1)<MAX_OBJECTS; i++ )
				{
					config_setting_t *an_object_cfg = config_setting_get_elem( objects_cfg, i );
					config_setting_lookup_float( an_object_cfg, "X", &object_list[i+1].x );
					config_setting_lookup_float( an_object_cfg, "Y", &object_list[i+1].y );
					config_setting_lookup_float( an_object_cfg, "R", &object_list[i+1].r );
					config_setting_lookup_int( an_object_cfg, "color", &object_list[i+1].color );
				}
			}
		}

		config_destroy( &config );
	}
	else
	{
		fprintf( stderr, "%s:%d - %s\n", config_error_file(&config),
										 config_error_line(&config),
										 config_error_text(&config) );

		config_destroy( &config );

		exit( ERR_CONFIG_FILE_ERROR );
	}
}
