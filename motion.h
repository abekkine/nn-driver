#ifndef MOTION_H
#define MOTION_H

void motion_init();
void motion_update();
void motion_compute_cell_values();
void motion_use_cell_values();
void motion_reset_cell_values();

extern int motion_num_motion;
extern float *motion_cells;
extern double motion_max_delta;
extern int motion_mouse_move_enable;

#endif
