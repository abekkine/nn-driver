#ifndef DISPLAY_H
#define DISPLAY_H

void display_init();
void display_update();
int display_check_quit();

extern int display_screen_width;
extern int display_screen_height;
extern unsigned int display_bg_color;

#endif

