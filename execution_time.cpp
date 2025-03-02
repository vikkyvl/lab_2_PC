//
// Created by User on 02.03.2025.
//

#include "execution_time.h"

ExecutionTime::ExecutionTime()
{
    start = std::chrono::high_resolution_clock::now();
}

ExecutionTime::~ExecutionTime()
{
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
}

