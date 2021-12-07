#include "thread.h"


/**
 * Set up the id, matrixSize, and all matrix.
 *
 */
void
Thread::initialThread (int program_id, int id, int matrix_size, int** single_result, 
                       int** multi_result, int** mul_matrix, int** input_matrix, int* shared_sum)
{
    programID = program_id;
    ID = id;
    matrixSize = matrix_size;

    singleResult = single_result;
    multiResult = multi_result;
    matrix = mul_matrix;
    inputMatrix = input_matrix;

    sharedSum = shared_sum;
}


void
Thread::setThreadCore(int core)
{ 
	setCore = core;
}


void
Thread::setStartCalculatePoint(int start_calculate_point)
{
    startCalculatePoint = start_calculate_point;
}


void
Thread::setEndCalculatePoint(int end_calculate_point)
{
    endCalculatePoint = end_calculate_point;
}


/**
 * Set up the affinity mask for the thread.
 *
 */
void
Thread::setUpCPUAffinityMask ()
{
	if (setCore == -1) {
        return;
    } else {
		cpu_set_t set;
		CPU_ZERO (&set);
		CPU_SET (setCore, &set);
		sched_setaffinity (0, sizeof (set), &set);
		setCore = -1;
	}
}


/**
 * Reset the matrix store value by copy the inputMatrix.
 *
 */
void
Thread::resetMatrix()
{
    for (int i = 0; i < matrixSize; i++)
        memcpy (matrix [i], inputMatrix [i], matrixSize * sizeof (int));
}


/**
 * Print out the tread information inclue ID, PID,
 * and execute core.
 *
 */
void
Thread::printInformation()
{
	core = sched_getcpu ();
	PID = syscall (SYS_gettid);
	pthread_mutex_lock (ioMutex);
    std::cout << "Program ID : " << programID;
    std::cout << "\tThread ID : " << ID;
    std::cout << "\tPID : " << PID;
    std::cout << "\tCore : " << core;
    std::cout << std::endl;
	pthread_mutex_unlock (ioMutex);
}


void
Thread::synchronize ()
{
#if SYNCHRONIZE == BARRIER
	/*~~~~~~~~~~~~Your code(PART1)~~~~~~~~~~~*/
	pthread_barrier_wait (barr);
	/*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/
#elif SYNCHRONIZE == SEMAPHORE
	for(int i = 0; i < THREAD_NUM; ++i){
		sem_post (&sem[i]);
	}
	for(int i = 0; i < THREAD_NUM; ++i){
		sem_wait(&sem[ID]);
	}
#else
	pthread_mutex_lock (ioMutex);
    std::cout << "Synchronize method not supported." << std::endl;
	pthread_mutex_unlock (ioMutex);
#endif
}


void 
Thread::enterCriticalSection ()
{
#if PROTECT_SHARED_RESOURCE == MUTEX
	/*~~~~~~~~~~~~Your code(PART1)~~~~~~~~~~~*/
	pthread_mutex_lock (ioMutex);
	/*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/
#elif PROTECT_SHARED_RESOURCE == SPINLOCK
	/*~~~~~~~~~~~~Your code(PART3)~~~~~~~~~~~*/
	pthread_spin_lock (spinlock);
	/*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/
#else
	pthread_mutex_lock (ioMutex);
    std::cout << "Synchronize method not supported." << std::endl;
	pthread_mutex_unlock (ioMutex);
#endif
}

void 
Thread::exitCriticalSection ()
{
#if PROTECT_SHARED_RESOURCE == MUTEX
	/*~~~~~~~~~~~~Your code(PART1)~~~~~~~~~~~*/
	pthread_mutex_unlock (ioMutex);
	/*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/
#elif PROTECT_SHARED_RESOURCE == SPINLOCK
	/*~~~~~~~~~~~~Your code(PART3)~~~~~~~~~~~*/
	pthread_spin_unlock (spinlock);
	/*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/
#else
	pthread_mutex_lock (ioMutex);
    std::cout << "Synchronize method not supported." << std::endl;
	pthread_mutex_unlock (ioMutex);
#endif
}

/**
 * Multi the array matrix to matrix for MULTI_TIME times.
 * After finishing multiplication, copy the result back to
 * matrix and do the multiplication agian.
 *
 */
void
Thread::singleMatrixMultiplication()
{	
	printInformation();

	// Multiplication for MULTI_TIME times
    for (int num_multi = 0; num_multi < MULTI_TIME; num_multi++) {

	    for (int i = 0 ; i < matrixSize; i++) {

	    	for (int j = 0 ; j < matrixSize; j++) {

                singleResult [i][j] = 0;
	    		for (int k = 0 ; k < matrixSize; k++)
	    			singleResult [i][j] += matrix [i][k] * matrix [k][j];

	    	} // for (int j...

	    } // for (int i...

        // Copy the singleResult back to matrix
        for (int i = 0; i < matrixSize; i++)
            memcpy (matrix [i], singleResult [i], matrixSize * sizeof (int));

    } // for (int num_multi...

}


/*
 * Matrix multiplication operation and store the result into
 * multieResult. 
 *
 */
void*
Thread::matrixMultiplication(void* args)
{
	/*~~~~~~~~~~~~Your code(PART1)~~~~~~~~~~~*/
    Thread *obj = (Thread*)args;

	obj->setUpCPUAffinityMask ();
	obj->printInformation ();
	
	// Multiplication for MULTI_TIME times
    for (int num_multi = 0; num_multi < MULTI_TIME; num_multi++) {

	    for (int i = obj->startCalculatePoint; i < obj->endCalculatePoint; i++) {

	    	for (int j = 0 ; j < obj->matrixSize; j++) {

#if (PART != 2)
				obj->enterCriticalSection();
                *obj->sharedSum = 0;
	    		for (int k = 0 ; k < obj->matrixSize; k++)
	    			*obj->sharedSum += obj->matrix [i][k] * obj->matrix [k][j];

                obj->multiResult [i][j] = *obj->sharedSum;
				obj->exitCriticalSection();
#else

	        /*~~~~~~~~~~~~Your code(PART2)~~~~~~~~~~~*/
				for (int k = 0 ; k < obj->matrixSize; k++)
					obj->multiResult [i][j] += obj->matrix [i][k] * obj->matrix [k][j];
	        /*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/

#endif


	    	} // for (int j...

	    } // for (int i...

		obj->synchronize ();
        // Copy the multiResult back to matrix
        for (int i = obj->startCalculatePoint; i < obj->endCalculatePoint; i++)
            memcpy (obj->matrix [i], obj->multiResult [i], obj->matrixSize * sizeof (int));


    } // for (int num_multi...

    return 0;
	/*~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~*/
}
