#ifndef PERCEPTION_H
#define PERCEPTION_H

void perception_init();
void perception_update();
void perception_loop();

extern float *perception_retina;
extern int perception_num_vision;

extern double perception_range_start;
extern double perception_range_end;
extern double perception_scan_step;

#endif

