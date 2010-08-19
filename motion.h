#ifndef MOTION_H
#define MOTION_H

void motion_init();
void motion_update();

extern float *motion_cells;
extern double motion_max_delta;

#endif
