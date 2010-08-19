#ifndef BRAIN_H
#define BRAIN_H

#define BRAIN_TRAINING_MODE 0
#define BRAIN_RUNNING_MODE 1
#define BRAIN_FREE_MODE 2

void brain_init();
void brain_update();
void brain_set_mode( int mode );
void brain_train();
void brain_run();
int brain_in_running_mode();
int brain_in_training_mode();

extern int brain_num_hidden;
extern int brain_num_layers;

#endif

