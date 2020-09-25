#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "agent.h"

class Environment {
  bool agent_bumped : 1;
  bool done : 1;
  bool init : 1;
  char ** env;
  size_t x; //environment x dimension
  size_t y; //environment y dimension
  size_t steps; //total steps/iterations so far
  Agent agent;
  size_t ax; //agent x
  size_t ay; //agent y
  size_t hx; //agent home x
  size_t hy; //agent home y
  Heading ah; //agent heading
  int score;

  public:
  Environment(char const* filename);
  ~Environment();
  bool AtHome() const;
  void Print() const;
  bool Done() const;
  void Step();

};
#endif
