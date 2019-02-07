#include "CLPMotor.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("start"));
  CLPMTR_initial();
  systme_parameter_initial(&SYS_state);
  interrupt_initial();
  Timer4_initial();
  Timer5_initial();
  delay(3000);     //delay wait for CLP motor and driver
  SYS_state.prevent_startup_into_ISR = true;
}

void loop() {
  switch (motor_set.motor_seq) {
    case 0:
      if (!motor_set.once) {
        Serial.println(F("case0"));
        CLPMTR_JogStepSet(motor_set.CW, 0);
        motor_set.once = true;
      }
      if (SYS_state.limit_sensor_state) {
        TimerStop();
        motor_set.motor_seq = 1;
        motor_set.once = false;
        SYS_state.limit_sensor_state = false;
        motor_set.pulse_count = 0;
      }
      break;
    case 1:
      //for delay
      if (!motor_set.once) {
        TIMSK5 = 0x01;       //timer5 start
        Serial.println(F("case1"));
        motor_set.once = true;
      }
      if (motor_set.delayOK) {
        Serial.println(F("case2 motor_set.delayOK"));
        motor_set.delayOK = 0;
        motor_set.motor_seq = 2;
        motor_set.once = false;
      }
      break;
    case 2:
      if (!motor_set.once) {
        Serial.println(F("case2"));
        CLPMTR_JogStepSet(motor_set.CCW, 0);
        motor_set.once = true;
        motor_set.pulse_count = 0;
      }
      break;

    case 3:
      //for delay
      if (!motor_set.once) {
        Serial.println(F("case3"));
        TIMSK5 = 0x01;       //timer5 start
        motor_set.once = true;
      }
      if (motor_set.delayOK) {
        Serial.println(F("case3  motor_set.delayOK"));
        motor_set.delayOK = 0;
        motor_set.motor_seq = 0;
        motor_set.once = false;
      }
      break;
  }
}


//timer set and CLPM StepSet
void CLPMTR_JogStepSet(bool CW_CCW, bool CLPM_arrive) {
  //Serial.println(F( "CLPMTR_JogStepSet"));
  set_motor_DIR(CW_CCW);
  TCNT4 = Timer4CountSet[motor_set.set_speed];
  TCNT5 = Timer5CountSet[timer5set];
  motor_set.arrive = CLPM_arrive;
  TimerStart();
  //Serial.println(F( "CLPMTR_JogStepSet over"));
}

//CLP motor initial
void CLPMTR_initial() {
  CLPM_tester->CLP_MOTOR_Initial(testerPUL, testerDIR);
  CLPM_tester->setCLPMTR_LOW();
  CLPM_tester->setCLPMTR_CW();
  motor_set_initial(&motor_set);
}


//timer4 initial
void Timer4_initial() {
  Timer4CountSet[0] = 65531; //0.02ms 20us中斷設定
  Timer4CountSet[1] = 65524; //0.048ms 48us中斷設定
  Timer4CountSet[2] = 65511; //0.1ms 100us中斷設定
  Timer4CountSet[3] = 65473; //0.25ms 250us中斷設定
  Timer4CountSet[4] = 65411; //0.5ms 500us中斷設定
  Timer4CountSet[5] = 65286; //1ms 1000us中斷設定
  Timer4CountSet[6] = 64286; //5ms 5000us中斷設定
  Timer4CountSet[7] = 63036; //10ms 中斷設定
  Timer4CountSet[8] = 53036; //50ms 中斷設定
  Timer4CountSet[9] = 40536; //100ms 中斷設定
  TCCR4A =  0x00;
  TCCR4B =  0X03;          //設定 除頻=3 16Mhz/64=0.25Mhz
  //1/0.25Mhz=4us,每4us計數一次,假設設定為65531,
  //共65536-65531=5,第5次發生計時中斷,共經時間5*4us=20us
  TCCR4C =  0x00;
  TIMSK4 =  0x00;      //timer4 stop
}

//timer5 initial
void Timer5_initial() {
  Timer5CountSet[0] = 65531; //20us中斷設定
  Timer5CountSet[1] = 65521;    //0.06ms 60us中斷設定
  Timer5CountSet[2] = 65511; //0.1ms 100us中斷設定
  Timer5CountSet[3] = 65473;  //0.25ms 250us中斷設定
  Timer5CountSet[4] = 65411;  //0.5ms 500us中斷設定
  Timer5CountSet[5] = 65286;   //1ms 1000us中斷設定
  Timer5CountSet[6] = 65036;   //2ms 2000us中斷設定
  TCCR5A =  0x00;
  TCCR5B =  0X03;          //除頻=5 16Mhz/1024 除頻=3 16Mhz/64
  TCCR5C =  0x00;
  TIMSK5 =  0x00;      //timer5 stop
}


//timer start
void TimerStart() {
  TIMSK4 = 0x01;       //timer4 start
  TIMSK5 = 0x01;       //timer5 start
}

//timer stop
void TimerStop() {
  TIMSK4 = 0x00;     //timer4 stop
  TIMSK5 = 0x00;     //timer5 stop
}

//timer4 interrput ISR
//create Pulse for CLPMOTOR driver
ISR (TIMER4_OVF_vect) {
  TIMSK4 = 0x00;     //timer4 stop
  motor_set.TimerSW = ! motor_set.TimerSW;
  TCNT4 = Timer4CountSet[motor_set.set_speed];
  if (!motor_set.arrive) {
    if ( motor_set.TimerSW) {
      CLPM_tester->setCLPMTR_HIGH();
    }  else {
      CLPM_tester->setCLPMTR_LOW();
      motor_set.pulse_count = motor_set.pulse_count + 1;
    }
    TIMSK4 = 0x01;       //timer4 start
  } else {
    CLPM_tester->setCLPMTR_LOW();
    TIMSK4 = 0x00;     //timer4 stop
  }
}

//timer5 interrput ISR
ISR (TIMER5_OVF_vect) {
  TIMSK5 = 0x00;     //timer5 stop
  TCNT5 = Timer5CountSet[timer5set];
  if ( motor_set.motor_seq == 1 || motor_set.motor_seq == 3) {
    motor_set.delay_count = motor_set.delay_count + 1;
    if (motor_set.delay_count == delaytime) {
      motor_set.delay_count = 0;
      motor_set.arrive = 1;
      motor_set.delayOK = 1;
    } else {
      motor_set.arrive = 0;
    }

  } else if (  motor_set.motor_seq == 2 ) {
    if (motor_set.pulse_count ==  motor_set.set_pulse)  {
      motor_set.arrive = 1;
      motor_set.pulse_count = 0;
      motor_set.motor_seq = 3;
      motor_set.once = false;
    }
  }

  if (!motor_set.arrive) {
    TIMSK5 = 0x01;       //timer5 start
  } else {
    TIMSK5 = 0x00;     //timer5 stop
  }
}


void set_motor_DIR(bool DIR) {
  if (DIR == motor_set.CW) {
    motor_set.DIR_state = CLPM_tester->setCLPMTR_CW();
  } else if (DIR == motor_set.CCW) {
    motor_set.DIR_state = CLPM_tester->setCLPMTR_CCW();
  }
}

void interrupt_initial() {
  //digitalWrite(control_BTN, HIGH); //turn on pullup resistors
  pinMode(CLPMR_limit_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLPMR_limit_pin ), limit_sensor_ISR, FALLING);
}

void limit_sensor_ISR() {
  if (SYS_state.prevent_startup_into_ISR) {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 200 && motor_set.motor_seq == 0) {
      SYS_state.limit_sensor_state = true;
      Serial.println(F( "limit_sensor_ISR"));
    }
    last_interrupt_time = interrupt_time;
  }
}

