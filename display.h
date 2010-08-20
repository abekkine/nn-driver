#ifndef DISPLAY_H
#define DISPLAY_H

#define KEY_UP_STATE 0
#define KEY_DOWN_EVENT 1
#define KEY_UP_EVENT 2
#define KEY_DOWN_STATE 3

#include <SDL/SDL.h>

void display_init();
void display_update();
void display_select_font( const char *font, int size );
int display_check_quit();
int display_init_font();
int display_poll_events();
void display_init_gl();
int display_init_screen();
void display_reshape( int width, int height );
int display_check_key( int key );
void display_text( int x, int y, const char *str );
void display_render();
void display_test_pattern();
void display_objects();
void display_object( double x, double y, double r, int color );
void display_convert_color( int rgb, float *r, float *g, float *b );
void display_perception_ring();
void display_motion_ring();
void display_keyevent( SDL_KeyboardEvent *key );
void display_get_pixel_color( double x, double y, float *red, float *green, float *blue );
unsigned int display_check_pixel( double x, double y );
void display_info();
void display_update_info();
void display_add_info( double x, double y, const char *str, int *enable_flag );

extern int display_screen_width;
extern int display_screen_height;
extern int display_bg_color;
extern int display_mouse_x;
extern int display_mouse_y;

#endif

