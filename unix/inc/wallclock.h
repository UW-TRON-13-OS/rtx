#ifndef WALLCLOCK_H
#define WALLCLOCK_H

void start_wallclock();

//get and set the clock's time 
int setWallClock (char* time);

//set whether clock is displayed or not
void displayWallClock (int disp_b); 

#endif
