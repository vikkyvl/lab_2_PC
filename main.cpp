#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <atomic>

#define SIZE 6
#define NUM_THREADS 4

#define MIN 1
#define MAX 100

int array[SIZE];

int getRandomNumber()
{
    return MIN + (rand() % (MAX - MIN + 1));
}

void fillArray()
{
    for(int& i : array)
    {
        i = getRandomNumber();
    }
}

void printArray()
{
    for(int i : array)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void calculateSequantialXOR(int& result)
{
    for(int i : array)
    {
        if((i % 2) == 0) //(i & 1)
        {
            result ^= i;
        }
    }
}

int main()
{
    srand(time(NULL));

    int sequantial_xor_result = 0;

    fillArray();
    //printArray();

    calculateSequantialXOR(sequantial_xor_result);
    //std::cout << sequantial_xor_result << std::endl;

    return 0;
}


