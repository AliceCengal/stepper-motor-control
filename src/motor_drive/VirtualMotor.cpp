#include <Arduino.h>
#include "VirtualMotor.h"

typedef struct {
  int runtime;
  int pos;
} RunState;

RunState* runState;

void showRunState() {
  Serial.print(runState->runtime);
  Serial.print(": ");
  for (int i = 0; i < (runState->pos / 25); ++i) {
    Serial.print("|");
  }
  Serial.println();
}

void printState() {
  Serial.print(runState->runtime);
  Serial.print(": ");
  Serial.println(runState->pos);
}

void VM_doCommand(Command* c) {
  
  int stepsRemaining = c->steps;
  
  while (stepsRemaining > 0) {
    runState->runtime += 1;
    if (c->dir == DIR_CW) {
      runState->pos += c->motorSpeed;
    } else if (c->dir == DIR_CCW) {
      runState->pos -= c->motorSpeed;
    }
    
    if (runState->pos < 0) {
      runState->pos = 2000 + runState->pos;
    }
    
    if (runState->pos > 2000) {
      runState->pos = runState->pos - 2000;
    }
    
    stepsRemaining -= c->motorSpeed;
    showRunState();
  }
  
}

void runVirtualMotor(int initialPosition, Command* c) {
  
  Serial.println("Virtual Motor running");
  
  runState = new RunState();
  runState->pos = initialPosition;
  runState->runtime = 0;
  
  showRunState();
  traverseCommands(c, &VM_doCommand);
  delete runState;
}


