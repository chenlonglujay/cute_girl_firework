#include "CGF.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   CGFW.cute_girl_firework_initial(PUL_pin, DIR_pin, riseHand_heat_BTN_pin,
                                  relay_heat_pin, limit_sensor_pin, heat_time, no_heat_time,
                                  step_goal);
  CGFW.interrupt_initial(limit_sensor_ISR);
  CGFW.set_prevent_startup_into_ISR(true);
  delay(2000);     //delay wait for CLP motor and driver
   Serial.println(F("start"));
}

void loop() {
  // put your main code here, to run repeatedly:
 all_action();
}

//timer4 interrput ISR
//create Pulse for CLPMOTOR driver
ISR (TIMER4_OVF_vect) {
  CGFW.timer4_ISR();
}

//timer5 interrput ISR
//for judge pulse number or calculate time
ISR (TIMER5_OVF_vect) {
  CGFW.timer5_ISR();
}

void limit_sensor_ISR() {
  if (CGFW.get_prevent_startup_into_ISR()) {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 200 && CGFW.get_action_seq() == 0) {
      CGFW.set_limit_sensor_state(true);
      Serial.println(F( "limit_sensor_ISR"));
    }
    last_interrupt_time = interrupt_time;
  }
}
    
void all_action() {
  
  switch (CGFW.get_action_seq()) {
    case 0:
      if (!CGFW.get_do_once()  && !CGFW.get_limit_sensor_state()) {
        Serial.println(F("case0"));
        CGFW.CLPMTR_setting(CW, 0);
        CGFW.set_do_once(true);
      }
      if (CGFW.get_limit_sensor_state()) {
        CGFW.timer4_stop();
        CGFW.set_action_seq(1);
        CGFW.set_do_once(false);
        CGFW.set_limit_sensor_state(false);
        CGFW.clear_motor_pulse_count();
        CGFW.timer5_start();
      }
      break;

    case 1:
      //delay 1s
      if (!CGFW.get_do_once()) {
        Serial.println(F("case1"));
         CGFW.set_do_once(true);
      }
      if (CGFW.get_delay_ok()) {
        Serial.println(F("case1 delay 1s OK"));
        CGFW.set_delay_ok(false);
        CGFW.set_action_seq(2);
        CGFW.set_do_once(false);
        CGFW.timer5_stop();
      }
      break;
     
    case 2:
     if(!CGFW.get_riseHand_heat_BTN_state()) {
         //Keeping Buttons in a Pressed State
         CGFW.set_BHST(pressed); 
          if (!CGFW.get_do_once()) {
              Serial.println(F("case2"));
              CGFW.CLPMTR_setting(CCW, 0);
              CGFW.set_do_once(true);            
          }
           
          if(CGFW.get_heat_action() == heating) {
            CGFW.turn_on_relay_heat(true);
            //Serial.println(F("case2 heating"));
          } else if(CGFW.get_heat_action() == no_heating_temporarily) {
            CGFW.turn_on_relay_heat(false);
            //Serial.println(F("case2 no_heating_temporarily"));
          }
          
       }  else if(CGFW.get_riseHand_heat_BTN_state() && CGFW.get_BHST() != no_press) {
            CGFW.set_action_seq(0);
            CGFW.set_BHST(no_press); 
            CGFW.set_do_once(false);
            CGFW.clear_motor_pulse_count();
            CGFW.turn_on_relay_heat(false);
            CGFW.set_heat_action(no_heating);
       }
      break;
  }
}




