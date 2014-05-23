
#include <iostream>
#include "VirtualMotor.h"

typedef struct {
  int runtime;
  int position;
} RunState;

RunState* runState;

void showRunState() {
  std::cout << runState->runtime << ": ";
  for (int i = 0; i < (runState->position / 10); ++i) {
    std::cout << "|";
  }
  std::cout << std::endl;
}

void VM_doCommand(Command* c) {
  
  int stepsRemaining = c->steps;
  
  while (stepsRemaining > c->motorSpeed) {
    runState->runtime += 1;
    if (c->dir == DIR_CW) {
      runState->position += c->motorSpeed;
    } else if (c->dir == DIR_CCW) {
      runState->position -= c->motorSpeed;
    }
    
    if (runState->position < 0) {
      runState->position = 200 - runState->position;
    }
    
    stepsRemaining -= c->motorSpeed;
    showRunState();
  }
  
}

void runVirtualMotor(int initialPosition, Command* c) {
  
  std::cout << "Virtual Motor running" << std::endl;
  
  runState = new RunState();
  runState->position = initialPosition;
  runState->runtime = 0;
  
  showRunState();
  traverseCommands(c, &VM_doCommand);
  delete runState;
}

