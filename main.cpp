#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <atomic>
#include "execution_time.h"

#define SIZE 100000000
#define NUM_THREADS 4
#define MIN 1
#define MAX 100

int array[SIZE];

std::mutex mtx;

int sequantial_xor_result = 0;
int synchronized_xor_result = 0;
std::atomic<int> atomic_xor_result(0);

enum Method {MUTEX, ATOMIC};

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

void calculateSequantialXOR()
{
    for(int i : array)
    {
        if((i % 2) == 0) //(i & 1)
        {
            sequantial_xor_result ^= i;
        }
    }
}

void sumWithMutex(int value)
{
    if(value % 2 == 0)
    {
        mtx.lock(); //std::lock_guard<std::mutex> lock(mtx);
        synchronized_xor_result ^= value;
        mtx.unlock();
    }
}

void sumWithAtomic(int value)
{
    int old_value, new_value;

    if(value % 2 == 0)
    {
        do
        {
            old_value = atomic_xor_result.load();
            new_value = old_value ^ value;
        }while(!atomic_xor_result.compare_exchange_weak(old_value, new_value));
    }
}

void elementDistribution(const int thread_id, Method method)
{
    for(int i = thread_id; i < SIZE; i+= NUM_THREADS)
    {
        if (method == MUTEX)
        {
            sumWithMutex(array[i]);
        }
        else if (method == ATOMIC)
        {
            sumWithAtomic(array[i]);
        }
    }
}

void calculateXOR(Method method)
{
    std::thread threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threads[i] = std::thread(elementDistribution, i, method);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }
}

int main()
{
    srand(time(NULL));

    fillArray();
    //printArray();

    std::cout << "\nArray size: " << SIZE << std::endl;
    std::cout << "\n* Sequential *" << std::endl;
    {
        ExecutionTime execution_time;
        calculateSequantialXOR();
    }
    std::cout << "Calculation result: " << sequantial_xor_result << std::endl;

    std::cout << "\n* Mutex *" << std::endl;
    {
        ExecutionTime execution_time;
        calculateXOR(MUTEX);
    }
    std::cout << "Calculation result: " << synchronized_xor_result << std::endl;

    std::cout << "\n* Atomic *" << std::endl;
    {
        ExecutionTime execution_time;
        calculateXOR(ATOMIC);
    }
    std::cout << "Calculation result: " << atomic_xor_result << std::endl;

    return 0;
}


