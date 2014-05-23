
#include "MotorCommand.h"

#define CCW_PIN 11
#define CW_PIN 12

void doSteps(int pin, int steps, int delayInterval) {
  for (int i = 0; i < steps; ++i) {
    digitalWrite(pin, HIGH);
    delay(delayInterval);
    digitalWrite(pin, LOW);
    delay(delayInterval);
  }
}
  
void doCommand(Command* c) {
  int steps = c->steps;
  int pin = (c->dir == DIR_CW) ? CW_PIN : CCW_PIN;
  doSteps(pin, steps, (c->motorSpeed)/2);
}

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  Command* command =
    combine(
      fastCCW(90),
      fastCCW(180));
      
  traverseCommands(command, &doCommand);
}

void loop() {
}


