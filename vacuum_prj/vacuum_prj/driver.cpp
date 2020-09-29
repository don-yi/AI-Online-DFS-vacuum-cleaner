#include <iostream>
#include "definitions.h"
#include "agent.h"
#include "environment.h"


int main (int , char** argv) {
  Environment e("1.txt");

  while (!e.Done()) {
    //e.Print();
    e.Step();
  }
  e.Print();
}
