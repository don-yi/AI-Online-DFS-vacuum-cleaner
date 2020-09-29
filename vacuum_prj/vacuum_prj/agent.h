#ifndef AGENT_H
#define AGENT_H

#include <utility>
//#include <unordered_map> // need custom hash function
#include <map>
#include <stack>

#include "definitions.h"


class Agent {
  public:
    Agent(int random_seed);
    Action GetAction(Percept p);


private:

    // priv tables
  // unexplored opt's for each pt
  std::map<std::pair<int, int>, std::stack<Action>> act;
  // pt stack for back-track
  std::stack<std::pair<int, int>> backtrack;


    // priv val's
  // pos trackers
  std::pair<int, int> prevPos;
  std::pair<int, int> currPos;

  Action prevAct{ NOOP }; // mainly root checker
 // custom headings according to init heading
  enum CustHead { ERR = -1, CUST_FWD, CUST_R, CUST_B, CUST_L, SIZE };
  int currHead{ 0 };   // custom heading tracker, init'ed fwd

    // checkers
  bool isBacktracking{ false };
  bool canTraverse{ true }; // from node w/ not exhausted branch


    // priv fn's
  void GenDefaultActs();
  std::pair<int, int> UPCurrPos(std::pair<int, int> pos);
  void Backtrack();
};

#endif

