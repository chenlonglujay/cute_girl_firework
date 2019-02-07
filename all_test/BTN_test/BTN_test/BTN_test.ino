#define Heat_RiseHand_pin 26
#define LED_Servo_pin 27

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Heat_RiseHand_pin,INPUT);
  pinMode(LED_Servo_pin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool HR_BTN = digitalRead(Heat_RiseHand_pin);
  bool LS_BTN = digitalRead(LED_Servo_pin);
  if(!HR_BTN) {
    Serial.println("Heat_RiseHand");
  }
  if(!LS_BTN) {
    Serial.println("LED_Servo");
  }
}
