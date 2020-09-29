#include <ctime> //for random seed
#include <cmath> 
#include "agent.h"

Agent::Agent(int random_seed) { 
  //supplying your own seed may help debugging, same seed will cause 
  //same random number sequence 
  if (random_seed==0) std::srand( static_cast<unsigned>(std::time(0))); // random seed from time
  else                std::srand( random_seed ); // random seed from user
} 

Action Agent::GetAction(Percept p) {
  // if dirt suck
  if (p.dirt) return SUCK;

  // ending cond: if home and done back-tracking, shut off
  if (
    prevAct == FORWARD
    and p.home
    and act[std::make_pair(0, 0)].empty()
    ) return SHUTOFF;

  // if bumped, ch curr pos to prev
  if (p.bump) currPos = prevPos;

  // if brand new pos,
  std::map<std::pair<int, int>, std::stack<Action>>::iterator findIttr;
  findIttr = act.find(currPos);
  if (findIttr == act.end()) {
    GenDefaultActs();
    backtrack.push(currPos);
  }

  // try new pos
  std::pair<int, int> newPos;
  if (
    not act[currPos].empty()
    and act[currPos].top() == FORWARD
    and not isBacktracking
    ) {
    // get expected new pos
    Action nextAct = act[currPos].top();
    newPos = UPCurrPos(currPos);

    // if new pos is already visited and goind fwd
    findIttr = act.find(newPos);
    if (nextAct == FORWARD and findIttr != act.end())
      // skip a foward act
      act[currPos].pop();
  }

  // backtrack cond
  if (act[currPos].empty()) {
    // ending cond: goal achieved (at home w/o any act's
    if (p.home) return SHUTOFF;
    else Backtrack();
  }
  // if more traversal after back track (act's not exhausted)
  else if (isBacktracking and not act[currPos].empty() and canTraverse) {
    // rotate to original
    act[currPos].push(RIGHT);
    act[currPos].push(RIGHT);

    // set bool back
    isBacktracking = false;
  }

  // get next act from stack
  Action nextAct = act[currPos].top();
  act[currPos].pop();

  // up curr heading
  if (nextAct == RIGHT) {
    ++currHead;
    if (currHead >= SIZE) currHead = CUST_FWD;
  }
  else if (nextAct == LEFT) {
    --currHead;
    if (currHead <= ERR) currHead = CUST_L;
  }

  // up priv val's
  prevPos = currPos;
  if (nextAct == FORWARD) {
    if (isBacktracking) {
      currPos = backtrack.top();
      canTraverse = true;
    }
    else if (newPos != std::make_pair(0, 0))
      currPos = newPos;
  }
  prevAct = nextAct;

  return nextAct;
}

// gen new acts for new pos found
void Agent::GenDefaultActs()
{
    // new possible actions
    std::stack<Action> newAction;
    newAction.push(FORWARD);
    newAction.push(RIGHT);
    newAction.push(RIGHT);
    newAction.push(FORWARD);
    newAction.push(LEFT);
    newAction.push(FORWARD);

    // push to act ls
    act.insert(
      std::pair<std::pair<int, int>, std::stack<Action>>(currPos, newAction)
    );
}

// get expected new pos w/ fwd act using prev pos and heading
std::pair<int, int> Agent::UPCurrPos(std::pair<int, int> pos)
{
  std::pair<int, int> newPos = pos;

  switch (currHead) {
  case ERR :
    abort();
  case CUST_FWD :
    ++std::get<0>(newPos);
    break;
  case CUST_B :
    --std::get<0>(newPos);
    break;
  case CUST_L :
    --std::get<1>(newPos);
    break;
  case CUST_R :
    ++std::get<1>(newPos);
    break;
  case SIZE :
    abort();
  }

  return newPos;
}

// gen act's w/ mv info (grid ind diff & heading)
void Agent::Backtrack()
{
  // get backtrack pos
  std::pair<int, int> backtracked = backtrack.top();
  // handle first time backtrack
  if (backtracked == currPos)
  {
    backtrack.pop();
    backtracked = backtrack.top();
  }

  // get diff in pos's
  std::pair<int, int> diff;
  std::get<0>(diff) = std::get<0>(backtracked) - std::get<0>(currPos);
  std::get<1>(diff) = std::get<1>(backtracked) - std::get<1>(currPos);

  // gen new act's according to pos diff and heading
  switch (currHead)
  {
  case CUST_FWD:
    if (std::get<0>(diff))
      act[currPos].push(FORWARD);
    else if (std::get<1>(diff) == -1) {
      act[currPos].push(FORWARD);
      act[currPos].push(LEFT);
    }
    else if (std::get<1>(diff) == 1) {
      act[currPos].push(FORWARD);
      act[currPos].push(RIGHT);
    }
    break;
  case CUST_R:
    if (std::get<1>(diff)) {
      act[currPos].push(FORWARD);
    }
    else if (std::get<0>(diff) == -1) {
      act[currPos].push(FORWARD);
      act[currPos].push(RIGHT);
    }
    else if (std::get<0>(diff) == 1) {
      act[currPos].push(FORWARD);
      act[currPos].push(LEFT);
    }
    break;
  case CUST_B:
    if (std::get<0>(diff)) {
      act[currPos].push(FORWARD);
    }
    else if (std::get<1>(diff) == -1) {
      act[currPos].push(FORWARD);
      act[currPos].push(RIGHT);
    }
    else if (std::get<1>(diff) == 1) {
      act[currPos].push(FORWARD);
      act[currPos].push(LEFT);
    }
    break;
  case CUST_L:
    if (std::get<1>(diff)) {
      act[currPos].push(FORWARD);
    }
    else if (std::get<0>(diff) == -1) {
      act[currPos].push(FORWARD);
      act[currPos].push(LEFT);
    }
    else if (std::get<0>(diff) == 1) {
      act[currPos].push(FORWARD);
      act[currPos].push(RIGHT);
    }
    break;
  }

  // set priv bool's
  isBacktracking = true;
  canTraverse = false;
}

