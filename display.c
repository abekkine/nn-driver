#include <SDL/SDL.h>
#include <GL/gl.h>
#include <FTGL/ftgl.h>

#include "object.h"
#include "display.h"

// Global Variables
int display_screen_width;
int display_screen_height;
int display_bg_color;

// Local Variables
Uint8 *_keys;
int _testing_mode_enabled;
float _bg_red;
float _bg_green;
float _bg_blue;
int _text_support;
char _font_name[256];
int _font_size;
FTGLfont *_font;

SDL_Surface *_screen;

void display_init()
{
	_text_support = 0;

	display_select_font( "freemono.ttf", 16 );
	display_init_screen( display_screen_width, display_screen_height );

	display_convert_color( display_bg_color, &_bg_red, &_bg_green, &_bg_blue );
}

void display_convert_color( int rgb, float *r, float *g, float *b )
{
	*r = ((rgb & 0xff0000) >> 16) / 255.0;
	*g = ((rgb & 0xff00) >> 8) / 255.0;
	*b = (rgb & 0xff) / 255.0;
}

int display_init_screen()
{
	int result = 0;
	int rc;

	rc = SDL_Init( SDL_INIT_VIDEO );
	if( rc < 0 )
	{
		fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
	}
	else
	{
		atexit( SDL_Quit );

		_screen = SDL_SetVideoMode( display_screen_width, display_screen_height, 16, SDL_OPENGL );
		if( _screen == NULL )
		{
			fprintf( stderr, "Unable to set %dx%d video mode: %s\n", display_screen_width, display_screen_height, SDL_GetError() );
		}
		else
		{
			if( 0 == display_init_font() )
			{
				_text_support = 1;
			}
			else
			{
				fprintf( stderr, "Font cannot be initialized.\n" );
			}

			display_init_gl();

			display_reshape( _screen->w, _screen->h );

			result = 1;
		}
	}
	
	return result;
}

void display_select_font( const char *font, int size )
{
	strcpy( _font_name, font );
	_font_size = size;
}

void display_update()
{
	display_render();
	display_poll_events();
	display_process_events();
}

void display_render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// TODO
	if( _testing_mode_enabled )
	{
		display_test_pattern();
	}
	else
	{
		display_objects();
	}

	SDL_GL_SwapBuffers();
}

int display_check_quit()
{
	int result = 0;

	if( display_check_key( SDLK_ESCAPE ) || display_check_key( SDLK_q ) )
	{
		result = 1;
	}

	return result;
}

void display_reshape( int width, int height )
{
	if( height == 0 )
	{
		height = 1;
	}

	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho( 0.0f, width, height, 0.0f, -1.0, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int display_init_font()
{
	int result = 0;

	_font = ftglCreatePixmapFont( _font_name );

	if( _font )
	{
		ftglSetFontFaceSize( _font, _font_size, _font_size );
	}

	return result;
}

void display_init_gl()
{
	glShadeModel( GL_SMOOTH );
	glClearColor( _bg_red, _bg_green, _bg_blue, 0.0 );
}

int display_poll_events()
{
	int result = 0;

	SDL_Event event;

	while( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_VIDEORESIZE:
				_screen = SDL_SetVideoMode( event.resize.w, event.resize.h, 16, SDL_OPENGL|SDL_RESIZABLE );
				if( _screen )
				{
					display_reshape( _screen->w, _screen->h );
				}
				break;

			case SDL_QUIT:
				result = 1;
				break;
		}
	}

	return result;
}

int display_check_key( int key )
{
	int result = 0;
	_keys = SDL_GetKeyState( NULL );

	if( _keys[ key ] )
	{
		result = 1;
	}

	return result;
}

void display_text( int x, int y, const char *str )
{
	if( _text_support )
	{
		glRasterPos2i( x, y );
		ftglRenderFont( _font, str, FTGL_RENDER_ALL );
	}
}

int display_key_state( int state )
{
	static int state_history = 0;

	state_history <<= 1;
	state_history |= (state & 1);
	state_history &= 3;

	return state_history;
}

void display_process_events()
{
	_keys = SDL_GetKeyState( NULL );

	if( display_key_state( _keys[ SDLK_t ] ) == KEY_UP_EVENT )	_testing_mode_enabled ^= 1;
}

void display_objects()
{
	int i;

	glColor3f( 1.0, 1.0, 1.0 );
	display_text( 20, 20, "Object Display" );

	for( i=0; i<=object_num; i++ )
	{
		display_object( object_list[i].x, object_list[i].y, object_list[i].r, object_list[i].color );
	}
}

void display_test_pattern()
{
	glColor3f( 1.0, 1.0, 1.0 );
	display_text( 20, 20, "Test Pattern" );

	glPointSize( 2.0 );
	glBegin( GL_POINTS );
		glVertex2d( 100.0, 100.0 );
	glEnd();
}

void display_object( double x, double y, double r, int color )
{
	float red, green, blue;

	display_convert_color( color, &red, &green, &blue );

	glColor3f( red, green, blue );

	glPointSize( 2.0 );
	glBegin( GL_POINTS );
		glVertex2d( x, y );
	glEnd();	
}

