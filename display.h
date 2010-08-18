#ifndef DISPLAY_H
#define DISPLAY_H

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

extern int display_screen_width;
extern int display_screen_height;
extern int display_bg_color;

#endif

