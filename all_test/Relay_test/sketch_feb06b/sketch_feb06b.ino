#define relay_led_pin 38
#define relay_fire_pin 37

void setup() {
  // put your setup code here, to run once:
  pinMode(relay_led_pin, OUTPUT);
  pinMode(relay_fire_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(relay_led_pin, 0);      //relay off
 digitalWrite(relay_fire_pin, 0);      //relay off
  delay(2000);
  digitalWrite(relay_led_pin, 1);      //relay on
  digitalWrite(relay_fire_pin, 1);      //relay off
  delay(2000);
}
