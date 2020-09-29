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

  // if home and done back-tracking, shut off
  if (
    prevAct == FORWARD
    and p.home
    and act[std::make_pair(0, 0)].empty()
    ) return SHUTOFF;

  // if bumped, ch curr pos to prev
  if (p.bump) currPos = prevPos;

  //++test;

  // if brand new pos,
  std::map<std::pair<int, int>, std::stack<Action>>::iterator findIttr;
  findIttr = act.find(currPos);
  if (findIttr == act.end()) {
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

    // push to track back
    backtrack.push(currPos);
  }

  // try new pos
  std::pair<int, int> newPos;
  if (
    not act[currPos].empty()
    and act[currPos].top() == FORWARD
    and not isBacktracking ) {
    Action nextAct = act[currPos].top();
    newPos = UPCurrPos(currPos);

    // if new pos is already visited but not backtracking
    findIttr = act.find(newPos);
    if (nextAct == FORWARD and findIttr != act.end())
      // skip a foward act
      act[currPos].pop();
  }

  // 1st time / back to backtrack cond
  if (act[currPos].empty()) {
    // goal achieved
    if (p.home) {
      return SHUTOFF;
    }
    //if (not isBacktracking) {
      //if (isFirstBacktrack) {
        //backtrack.pop();
        //isFirstBacktrack = false;
      //}
    //  Backtrack();
    //}
    // continuing backtrack cond
    //else
      Backtrack();
    //}
  }
  // if more traversal after track back
  else if (isBacktracking and not act[currPos].empty() and canTraverse) {
    // rotate to original
    act[currPos].push(RIGHT);
    act[currPos].push(RIGHT);

    // set back to is not bactrack
    isBacktracking = false;
  }

  // get next act from stack
  Action nextAct = act[currPos].top();
  act[currPos].pop();

  // up curr heading
  if (nextAct == RIGHT) {
    ++currHeading;
    if (currHeading >= SIZE) currHeading = F;
  }
  if (nextAct == LEFT) {
    --currHeading;
    if (currHeading <= ERR) currHeading = NEG;
  }

  // up priv val's
  prevPos = currPos;
  if (nextAct == FORWARD) {
    if (isBacktracking) {
      currPos = backtrack.top();
      canTraverse = true;
    }
    else if (newPos != std::make_pair(0, 0)) {
      //// push to track back
      //backtrack.push(currPos);
      currPos = newPos;
    }
  }
  prevAct = nextAct;

  // up prev pos
  return nextAct;
}

std::pair<int, int> Agent::UPCurrPos(std::pair<int, int> pos)
{
  std::pair<int, int> newPos = pos;

  switch (currHeading) {
  case ERR :
    abort();
  case F :
    ++std::get<0>(newPos);
    break;
  case B :
    --std::get<0>(newPos);
    break;
  case NEG :
    --std::get<1>(newPos);
    break;
  case POS :
    ++std::get<1>(newPos);
    break;
  case SIZE :
    abort();
  }

  return newPos;
}

void Agent::Backtrack()
{
  // get backtrack pos
  std::pair<int, int> backtracked = backtrack.top();
  if (backtracked == currPos)
  {
    backtrack.pop();
    backtracked = backtrack.top();
  }

  // get diff in pos's
  std::pair<int, int> diff;
  std::get<0>(diff) = std::get<0>(backtracked) - std::get<0>(currPos);
  std::get<1>(diff) = std::get<1>(backtracked) - std::get<1>(currPos);

  switch (currHeading)
  {
  case F:
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
  case POS:
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
  case B:
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
  case NEG:
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

  // set bool
  isBacktracking = true;
  canTraverse = false;
}

