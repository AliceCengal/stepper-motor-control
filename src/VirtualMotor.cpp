
#include <iostream>
#include "VirtualMotor.h"

typedef struct {
  int runtime;
  int position;
} RunState;

RunState* RUNSTATE;

void showRunState() {
  std::cout << RUNSTATE->runtime << ": ";
  for (int i = 0; i < (RUNSTATE->position / 10); ++i) {
    std::cout << "|";
  }
  std::cout << std::endl;
}

void printState() {
  std::cout << RUNSTATE->runtime << ": " 
            << RUNSTATE->position << std::endl;
}

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
  
}

void runVirtualMotor(int initialPosition, Command* c) {
  
  std::cout << "Virtual Motor running" << std::endl;
  
  RUNSTATE = new RunState();
  RUNSTATE->position = initialPosition;
  RUNSTATE->runtime = 0;
  
  showRunState();
  traverseCommands(c, &VM_doCommand);
  delete RUNSTATE;
}


