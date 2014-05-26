#include <Arduino.h>
#include "VirtualMotor.h"

typedef struct {
  int runtime;
  int pos;
  
  double partialTime;
  int partialSteps;
} RunState;

RunState* RUNSTATE;

void showRunState() {
  Serial.print(RUNSTATE->runtime);
  Serial.print(": ");
  for (int i = 0; i < (RUNSTATE->pos / 25); ++i) {
    Serial.print("|");
  }
  Serial.println();
}

void printState() {
  Serial.print(RUNSTATE->runtime);
  Serial.print(": ");
  Serial.println(RUNSTATE->pos);
}

void adjustPosition() {
  if (RUNSTATE->pos < 0) {
    RUNSTATE->pos = 2000 + RUNSTATE->pos;
  } 
  if (RUNSTATE->pos > 2000) {
    RUNSTATE->pos = RUNSTATE->pos - 2000;
  }
}

void doSteps(int dir, int steps, int motorSpeed) {
  int remainingSteps = steps;
  
  // HEAD
  if ((RUNSTATE->partialTime + (double(steps) / motorSpeed)) > 1.0) {
    int headSteps = (1 - RUNSTATE->partialTime) * motorSpeed;
    RUNSTATE->pos += (headSteps*dir + RUNSTATE->partialSteps);
    RUNSTATE->runtime += 1;
    RUNSTATE->partialSteps = 0;
    RUNSTATE->partialTime = 0;
    remainingSteps -= headSteps;
    
    adjustPosition();
    showRunState();
  }
  
  // BODY
  int fullTime = remainingSteps / motorSpeed; // seconds
  int partialSteps = remainingSteps % motorSpeed;
  
  for (int t = 0; t < fullTime; ++t) {
    RUNSTATE->runtime += 1;
    RUNSTATE->pos += (dir * motorSpeed);
    adjustPosition();
    showRunState();
  }
  
  // TAIL
  RUNSTATE->partialTime += double(partialSteps) / motorSpeed;
  RUNSTATE->partialSteps += dir * partialSteps;
}


void doDelay(int seconds) {
  for (int t = 0; t < seconds; ++t) {
    RUNSTATE->runtime += 1;
    showRunState();
  }
}

void VM_doCommand(Command* c) {
  if (c->dir == DIR_STATIONARY) {
    doDelay(c->steps);
  } else if (c->dir == DIR_CW) {
    doSteps(1, c->steps, c->motorSpeed);
  } else {
    doSteps(-1, c->steps, c->motorSpeed);
  }
}

void runVirtualMotor(int initialPosition, Command* c) {
  
  Serial.println("Virtual Motor running");
  
  RUNSTATE = new RunState();
  RUNSTATE->pos = initialPosition;
  RUNSTATE->runtime = 0;
  RUNSTATE->partialSteps = 0;
  RUNSTATE->partialTime = 0;
  
  showRunState();
  traverseCommands(c, &VM_doCommand);
  delete RUNSTATE;
}


