//hardware:
//MEGA 2560
//CLP MOTOR AND DRIVER
//function
//(1)CW (2)CCW
//use TIMER4  and TIMER5 to detect and create pulse
//CLPMotor driver(Hybird Servo Drive) Pulse needs to set 1600
//divide 1600,the smallest pulse is 8(5steps),so at least run once needs 5 steps
//go_step*pulseChange/10 
//5 * 16 /10=8step
//use Timer4 Timer5 for CLP MOTOR, 
//but RC servo needs Timer4 Timer5,so couldn't use above at sametime
#ifndef __CGF_H
#define __CGF_H
#include <CUTE_GIRL_FIREWORK.h>
#define limit_sensor_pin 2
#define riseHand_heat_BTN_pin 26
#define PUL_pin  23
#define DIR_pin 22

#define relay_heat_pin 37
#define heat_time 4000 //1ms*4000
#define no_heat_time 2000 //1ms*2000
#define step_goal 800            //for CCW,motor connects Reducer(1:4) 
CUTE_GIRL_FIREWORK CGFW;
#endif 
