#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "definitions.h"
#include "configuration.h"

static config_t config;
static config_setting_t *setting_cfg;
static FILE *f_config;

int screen_width;
int screen_height;
int bgColor;

int num_visual;
int num_hidden;
int num_motion;

double env_visibility;
double env_radius;

double selfX;
double selfY;
double selfR;
int selfColor;

//double objX;
//double objY;
//double objR;
//int objColor;


void configuration_dump()
{
	printf( "screen(%d,%d)\n", screen_width, screen_height );
	printf( "bgColor(%08x)\n", bgColor );
	printf( "num_visual(%d)\n", num_visual );
	printf( "num_hidden(%d)\n", num_hidden );
	printf( "num_motion(%d)\n", num_motion );
	printf( "env_visibility(%lf)\n", env_visibility );
	printf( "env_radius(%lf)\n", env_radius );
	printf( "selfX(%lf)\n", selfX );
	printf( "selfY(%lf)\n", selfY );
	printf( "selfR(%lf)\n", selfR );
	printf( "selfColor(%08x)\n", selfColor );
	printf( "objX(%lf)\n", objX );
	printf( "objY(%lf)\n", objY );
	printf( "objR(%lf)\n", objR );
	printf( "objColor(%08x)\n", objColor );
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
				config_setting_lookup_int( screen_cfg, "width", &screen_width );
				config_setting_lookup_int( screen_cfg, "height", &screen_height );
			}

			config_setting_lookup_int( setting_cfg, "background", &bgColor );
		}

		setting_cfg = config_lookup( &config, "nndriver.brain" );
		if( setting_cfg != NULL )
		{
			config_setting_lookup_int( setting_cfg, "num_visual", &num_visual );
			config_setting_lookup_int( setting_cfg, "num_hidden", &num_hidden );
			config_setting_lookup_int( setting_cfg, "num_motion", &num_motion );
		}

		setting_cfg = config_lookup( &config, "nndriver.environment" );
		if( setting_cfg != NULL )
		{
			config_setting_lookup_float( setting_cfg, "visibility", &env_visibility );

			config_setting_lookup_float( setting_cfg, "radius", &env_radius );

			config_setting_t *self_cfg = config_lookup( &config,  "nndriver.environment.self" );
			if( self_cfg != NULL )
			{
				config_setting_lookup_float( self_cfg, "X", &selfX );
				config_setting_lookup_float( self_cfg, "Y", &selfY );
				config_setting_lookup_float( self_cfg, "R", &selfR );
				config_setting_lookup_int( self_cfg, "color", &selfColor );
			}

			config_setting_t *objects_cfg = config_lookup( &config, "nndriver.environment.objects" );
			if( objects_cfg != NULL )
			{
				int object_count = config_setting_length( objects_cfg );
				int i;

				for( i=0; i<object_count && i<MAX_OBJECTS; i++ )
				{
					config_setting_t *an_object_cfg = config_setting_get_elem( objects_cfg, i );
					config_setting_lookup_float( an_object_cfg, "X", &objX );
					config_setting_lookup_float( an_object_cfg, "Y", &objY );
					config_setting_lookup_float( an_object_cfg, "R", &objR );
					config_setting_lookup_int( an_object_cfg, "color", &objColor );
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
