#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <cassert>
#include <fstream>
#include <sstream>
#include <sys/time.h>

#include <pthread.h>

#include "check.h"
#include "config.h"
#include "thread.h"
#include <semaphore.h>

typedef void* (*THREADFUNCPTR)(void*);


class System
{
  public:
    System (); 
    void setUpMatrix ();           // Initialize the all matrix
    void init (); 
    
    void singleCoreMatrixMulti (); // Single thread matrix multiplicaiton
    void multiCoreMatrixMulti ();  // Multi-thread matrix multiplication
    
    void setStartTime ();
    void setEndTime ();
    double period () { return timeUse; };
  
  private:
    int numThread;                 // Thread number of current system
    Thread** threadSet;            // List of thread
    
    int ***matrix;                 // Shared matrix for each thread
    int ***inputMatrix;            // Input data for matrix multiplication
    int ***singleResult;           // Single-core matrix multiplication result
    int ***multiResult;		       // Mulit-core matrix multiplication result

    struct timeval start;          // Store the start time
    struct timeval end;            // Store the end time
    double timeUse;                // Store the interval between start and end time

    Check* check;                  // Checker

    int* sharedSum;                // Shared resource

    static pthread_mutex_t ioMutex; // IO mutex
    static pthread_spinlock_t spinlock;
    static pthread_barrier_t* barr;
    static sem_t** sem;
};
#endif
