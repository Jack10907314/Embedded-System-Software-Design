#include "check.h"

void
Check::initialCheck (int program_id, int** single_result, int** multi_result, int matrix_size)
{
    programID = program_id;
    singleResult = single_result;
    multiResult  = multi_result;
    matrixSize   = matrix_size;
}


void
Check::compareResult()
{
    for (int row_index = 0; row_index < matrixSize; row_index++) {
    
        for (int col_index = 0; col_index < matrixSize; col_index++) {
    
    	    if (multiResult [row_index][col_index] != singleResult [row_index][col_index]) {
                std::cout << "Program-" << programID 
                    << " obtain wrong matrix multiplication result." << std::endl; 
                return;
            } // if

        } // for (int col_index ...

    } // for (int row_index ...

    std::cout << "Program-" << programID 
        << " obtain correct matrix multiplication result." << std::endl; 

}
