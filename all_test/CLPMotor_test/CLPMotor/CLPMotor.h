//--------------------------------------
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
//--------------------------------------

#ifndef __CLPMOTOR_LED_H
#define __CLPMOTOR_LED_H

#include <CLP_MOTOR.h>
#define CLPMR_limit_pin 2
//CLP STEP MOTOR setting
CLPMTR *CLPM_tester = new CLPMTR;
#define testerPUL  23
#define testerDIR  22
#define delaytime 600      //3000ms
//timer setting
unsigned int Timer4CountSet[10];
unsigned int Timer5CountSet[7];
#define timer5set 5

struct  motor_control{
  bool arrive;
  bool DIR_state;      //0 CW ,1 CCW
  bool TimerSW;      //pulse high low change
  uint8_t set_speed;
  int pulse_count;
  int set_step;
  int set_pulse;
  enum {CW = 0,CCW};
  uint8_t motor_seq;
  bool once;
  int delay_count;
  bool delayOK;
} motor_set;

struct system_state {
    bool limit_sensor_state;     //true: sensing something
    bool prevent_startup_into_ISR;
} SYS_state;


 void motor_set_initial(struct  motor_control *input){    
    input->arrive = true;
    input->DIR_state =  input->CW; 
    input->TimerSW = 0;      //pulse high low change
    input->set_speed = 5;
    input->pulse_count = 0;
    input->set_step = 1000;
    input->set_pulse =  input->set_step * 16 / 10;
    input->motor_seq = 0;
    input->once = 0;
    input->delay_count = 0;
    input->delayOK = 0;
}

void systme_parameter_initial(struct system_state *input) {
  input->limit_sensor_state = false;  //true: sensing something
  input->prevent_startup_into_ISR = false;
}

#endif
