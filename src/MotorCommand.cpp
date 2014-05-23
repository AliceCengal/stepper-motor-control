
#include "MotorCommand.h"

Command* fastCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_FAST;
  c->steps = steps;
  c->dir = DIR_CW;
  c->flag = FLAG_LINEAR;
  c->next = NULL;
  return c;
}

Command* fastCCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_FAST;
  c->steps = steps;
  c->dir = DIR_CCW;
  c->flag = FLAG_LINEAR;
  c->next = NULL;
  return c;
}

Command* slowCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_SLOW;
  c->steps = steps;
  c->dir = DIR_CW;
  c->flag = FLAG_LINEAR;
  c->next = NULL;
  return c;
}

Command* slowCCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_SLOW;
  c->steps = steps;
  c->dir = DIR_CCW;
  c->flag = FLAG_LINEAR;
  c->next = NULL;
  return c;
}

Command* stationary(int seconds) {
  Command* c = new Command();
  c->motorSpeed = 1;
  c->steps = seconds;
  c->flag = FLAG_LINEAR;
  c->next = NULL;
  return c;
}

Command* fastSmoothCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_FAST;
  c->steps = steps;
  c->dir = DIR_CW;
  c->flag = FLAG_SMOOTH;
  c->next = NULL;
  return c;
}

Command* fastSmoothCCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_FAST;
  c->steps = steps;
  c->dir = DIR_CCW;
  c->flag = FLAG_SMOOTH;
  c->next = NULL;
  return c;
}

Command* slowSmoothCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_SLOW;
  c->steps = steps;
  c->dir = DIR_CW;
  c->flag = FLAG_SMOOTH;
  c->next = NULL;
  return c;
}

Command* slowSmoothCCW(int steps) {
  Command* c = new Command();
  c->motorSpeed = SPEED_SLOW;
  c->steps = steps;
  c->dir = DIR_CCW;
  c->flag = FLAG_SMOOTH;
  c->next = NULL;
  return c;
}

Command* combine(Command* c1, Command* c2) {
  Command* c = c1;
  while (c->next != NULL) {
    c = c->next;
  }
  c->next = c2;
  return c1;
}

Command* repeat(int count, Command* c) {
  if (count < 2) {
    return c;
  } else {
    Command* initialCopy = copy(c);
    for (int i = 3; i <= count; ++i) {
      combine(initialCopy, copy(c));
    }
    combine(c, initialCopy);
  }
  return c;
}

Command* repeatForSeconds(int seconds, Command* c) {
  
}

Command* copy(Command* c) {
  Command* c2 = new Command();
  c2->motorSpeed = c->motorSpeed;
  c2->steps = c->steps;
  c2->dir = c->dir;
  c2->flag = c->flag;
  
  if (c->next != NULL) {
    c2->next = copy(c->next);
  } else {
    c2->next = NULL;
  }
  return c2;
}

void dispose(Command* c) {
  if (c->next != NULL) {
    dispose(c->next);
  }
  delete c;
}

void traverseCommands(Command* commands, void (*executor)(Command*)) {
  Command* curr = commands;
  while(curr != NULL) {
    executor(curr);
    curr = curr->next;
  }
}

int stepperPeriodForOmega(double omega) {
  return 1000 / (STEPS_REV * omega);
}
