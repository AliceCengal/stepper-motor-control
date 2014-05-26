
#include <iostream>
#include "VirtualMotor.h"

typedef struct {
  int runtime;
  int pos;
  double partialTime;
  int partialSteps;
} RunState;

RunState* RUNSTATE;

void showRunState() {
  std::cout << RUNSTATE->runtime << ": ";
  int offset = RUNSTATE->pos / 10 - 1;
  for (int i = 0; i < offset; ++i) {
    std::cout << " ";
  }
  std::cout << "o" << std::endl;
}

void printState() {
  std::cout << RUNSTATE->runtime << ": " 
            << RUNSTATE->pos << std::endl;
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
/*
void VM_doCommand(Command* c) {
  
  int stepsRemaining = c->steps;
  
  while (stepsRemaining > 0) {
    RUNSTATE->runtime += 1;
    if (c->dir == DIR_CW) {
      RUNSTATE->position += c->motorSpeed;
    } else if (c->dir == DIR_CCW) {
      RUNSTATE->position -= c->motorSpeed;
    }
    
    if (RUNSTATE->position < 0) {
      RUNSTATE->position = 2000 + RUNSTATE->position;
    }
    
    if (runState->position > 2000) {
      RUNSTATE->position = RUNSTATE->position - 2000;
    }
    
    stepsRemaining -= c->motorSpeed;
    showRunState();
  }
  
} */

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
  
  std::cout << "Virtual Motor running" << std::endl;
  std::cout << "   |          '          '          '          |          '          '          '          |" << std::endl;
  
  RUNSTATE = new RunState();
  RUNSTATE->pos = initialPosition;
  RUNSTATE->runtime = 0;
  RUNSTATE->partialSteps = 0;
  RUNSTATE->partialTime = 0;
  
  showRunState();
  traverseCommands(c, &VM_doCommand);
  delete RUNSTATE;
}


