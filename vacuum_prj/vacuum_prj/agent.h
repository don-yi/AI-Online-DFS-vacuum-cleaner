#ifndef AGENT_H
#define AGENT_H

#include <utility>
//#include <unordered_map>
#include <map>
#include <stack>

#include "definitions.h"


class Agent {
  public:
    Agent(int random_seed);
    Action GetAction(Percept p);


private:
  // unexplored opt's for each pt
  std::map<std::pair<int, int>, std::stack<Action>> act;
  // pt stack for back-track
  std::stack<std::pair<int, int>> backtrack;
  //// state for each pt
  //std::map<std::pair<int, int>, bool*> openingState;


  std::pair<int, int> prevPos;
  std::pair<int, int> currPos;

  Action prevAct{ NOOP };
  enum HeadingFromOrigin { ERR = -1, F, POS, B, NEG, SIZE };
  int currHeading{ 0 }; // F

  bool isBacktracking{ false };
  bool canTraverse{ true };
  //bool isFirstBacktrack{ true };

  std::pair<int, int> UPCurrPos(std::pair<int, int> pos);
  void Backtrack();

  //bool IsDeadEnd(std::pair<int, int> pos);
  int test{ 0 };
};

#endif

