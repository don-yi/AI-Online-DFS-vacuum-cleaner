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
  //straight-forward beahavior
  if (p.dirt) return SUCK;
  if (p.bump) return (std::rand() % 5 < 3) ? LEFT : RIGHT; // P(LEFT) = 3/5
  if (p.home) return (std::rand() % 2) ? SHUTOFF : FORWARD;
  //default rule
  return (std::rand() % 5 < 3) ? 
               FORWARD :
               ((std::rand() % 2) ? LEFT : RIGHT);
}

