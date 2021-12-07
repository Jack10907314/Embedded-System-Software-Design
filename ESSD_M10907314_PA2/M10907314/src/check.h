#ifndef _CHECK_H_
#define _CHECK_H_

#include <iostream>

class Check
{

  public:
    Check () {};
    void initialCheck (int, int**, int**, int);

    void compareResult ();

  private:
    int programID;
    int matrixSize;             // matrix for each thread
    int **singleResult;       // single-core matrix multiplication result for each thread
    int **multiResult;        // mulit-core matrix multiplication result for each thread
};

#endif

