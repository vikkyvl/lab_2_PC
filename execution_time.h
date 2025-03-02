//
// Created by User on 02.03.2025.
//

#ifndef EXECUTION_TIME_H
#define EXECUTION_TIME_H

#include <iostream>
#include <chrono>

class ExecutionTime
{
public:
    ExecutionTime();
    ~ExecutionTime();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
};

#endif //EXECUTION_TIME_H
