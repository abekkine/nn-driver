#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <FTGL/ftgl.h>

#include "object.h"
#include "brain.h"
#include "perception.h"
#include "display.h"

// Global Variables
int display_screen_width;
int display_screen_height;
int display_bg_color;

// Local Variables
Uint8 *_keys;
int _mouse_x;
int _mouse_y;
int _testing_mode_enabled;
int _perception_ring_enabled;
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
	_testing_mode_enabled = 0;
	_perception_ring_enabled = 1;

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

	self->x = _mouse_x;
	self->y = _mouse_y;
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

	display_info();

	SDL_GL_SwapBuffers();
}

void display_info()
{
	glColor3f( 1.0, 1.0, 1.0 );
	if( _testing_mode_enabled )
	{
		display_text( 20, 20, "Test Pattern" );
	}
	else 
	{
		display_text( 20, 20, "Object Display" );
	}

	if( _perception_ring_enabled )
	{
		char mouse_str[] = "X(0000) Y(0000)";
		glColor3f( 1.0, 1.0, 1.0 );
		sprintf( mouse_str, "X(%04d) Y(%04d)", _mouse_x, _mouse_y );
		display_text( 10, display_screen_height-20, mouse_str );
	}
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

	glOrtho( 0.0f, width, height, 0.0, -1.0, 1.0f );

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

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				display_keyevent( &event.key );
				break;

			case SDL_MOUSEMOTION:
				_mouse_x = event.motion.x;
				_mouse_y = event.motion.y;
				break;
		}
	}

	return result;
}

void display_keyevent( SDL_KeyboardEvent *key )
{
	if( key->type == SDL_KEYDOWN )
	{
		switch( key->keysym.sym )
		{
			case SDLK_t:
				_testing_mode_enabled ^= 1;
				break;

			case SDLK_p:
				_perception_ring_enabled ^= 1;
				break;

			default:
				break;
		}
	}
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

void display_objects()
{
	int i;

	for( i=0; i<=object_num; i++ )
	{
		display_object( object_list[i].x, object_list[i].y, object_list[i].r, object_list[i].color );
	}

	perception_loop();

	if( _perception_ring_enabled )
	{
		display_perception_ring();
	}
}

void display_test_pattern()
{
	glPointSize( 2.0 );
	glBegin( GL_POINTS );
		glVertex2d( 100.0, 100.0 );
	glEnd();
}

void display_object( double x, double y, double r, int color )
{
	double alpha;
	float red, green, blue;

	display_convert_color( color, &red, &green, &blue );

	glColor3f( red, green, blue );

	glBegin( GL_POLYGON );
		for( alpha = 0.0; alpha < 2.0*M_PI; alpha += 0.1*M_PI )
			glVertex2d( x + r*cos(alpha), y + r*sin(alpha) );
	glEnd();
}

void display_perception_ring()
{
	int i;
	double alpha;
	double scale = 2.0;

	glPointSize( 1.0 );
	glBegin( GL_POINTS );
	for( i=0; i<brain_num_visual; i++ )
	{
		alpha = 2.0 * i * M_PI / brain_num_visual;
		glColor3f( perception_retina[3*i+0], perception_retina[3*i+1], perception_retina[3*i+2] );
		glVertex2d( self->x + scale*self->r*cos(alpha), self->y + scale*self->r*sin(alpha) );
	}
	glEnd();
}

void display_get_pixel_color( double x, double y, float *red, float *green, float *blue )
{
	GLfloat pixels[3];

	glReadPixels( (int) x, (int)(display_screen_height-y), 1, 1, GL_RGB, GL_FLOAT, (void *) pixels );

	*red = pixels[0];
	*green = pixels[1];
	*blue = pixels[2];
}

unsigned int display_check_pixel( double x, double y )
{
	unsigned int buffer = 0;

	glReadPixels( (int) x, (int)(display_screen_height-y), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) &buffer );

	return buffer;
}

