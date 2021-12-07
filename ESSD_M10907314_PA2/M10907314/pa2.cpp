#include "src/system.h"

/*****************************************************
Compiling: make part#.out 

e.g.       make part1.out 
           make part2.out 
           make part3.out 

Execute	: ./part#.out <Input file>

e.g.      ./part1.out 
          ./part2.out
          ./part3.out 
******************************************************/

int main ()
{
    System sys;
    sys.singleCoreMatrixMulti (); // Single thread matrix multiplication
    sys.multiCoreMatrixMulti (); // Multi-thread matrix multiplicaiton

    return 0;
}
