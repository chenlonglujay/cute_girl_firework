#include <Servo.h>

#define servo_pin 24
#define servo_LED_BTN_pin 27
#define relay_LED_pin 38
#define servo_init 90
#define servo_limitR 150
#define servo_limitL 40
Servo servo;

void setup() {
  // put your setup code here, to run once:
    servo_LED_initial();
}

void loop() {
  // put your main code here, to run repeatedly:
  servo_LED_BTN();
}

void servo_LED_BTN() {
  
 if(!digitalRead(servo_LED_BTN_pin)) {
      digitalWrite(relay_LED_pin, 1);      //relay on
      servo.write(servo_limitL);
      Serial.println(F("can_control_servo_LED"));
      delay(5000);
  } else {
     digitalWrite(relay_LED_pin, 0);      //relay off
      servo.write(servo_limitR);
  }
}

 void servo_LED_initial() {
    servo.attach(servo_pin);
    pinMode(relay_LED_pin, OUTPUT);
    pinMode(servo_LED_BTN_pin, INPUT);
#if 1
//zero
    servo.write(servo_init);
        delay(10000);
#else if
    servo.write(servo_limitR);
      delay(2000);
#endif    
 }
