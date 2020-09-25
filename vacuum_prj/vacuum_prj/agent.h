#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"


class Agent {
  public:
    Agent(int random_seed);
    Action GetAction(Percept p);
};

#endif

