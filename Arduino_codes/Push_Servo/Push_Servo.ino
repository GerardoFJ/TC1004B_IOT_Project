// 180 para cerrar
// 0 para abrir
#include <Servo.h>

#define SERVO_PIN 12
#define BUTTON_PIN 13
Servo servo;  // create servo object to control a servo  // variable to store the servo position

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
    // attaches the servo on pin D7 to the servo object
}

void loop() {
                      // waits 10ms for the servo to reach the position

}