#include <Servo.h>
#define servo_pin 24
#define servo_init 90
#define servo_limitR 150
#define servo_limitL 40

Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servo_pin);
  servo.write(servo_init);
}

void loop() {
  // put your main code here, to run repeatedly:

    servo.write(servo_limitR);
    delay(2000);
    servo.write(servo_limitL);
    delay(2000);
}
