#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <atomic>

#define SIZE 1000
#define NUM_THREADS 4

#define MIN 1
#define MAX 100

int array[SIZE];
std::mutex mtx;

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

void sumWithMutex(int value, int& result)
{
    if(value % 2 == 0)
    {
        mtx.lock(); //std::lock_guard<std::mutex> lock(mtx);
        result ^= value;
        mtx.unlock();
    }
}

void elementDistributionMutex(const int thread_id, int& result)
{
    for(int i = thread_id; i < SIZE; i+= NUM_THREADS)
    {
        sumWithMutex(array[i], result);
    }
}

void calculateSynchronizedXOR(int& result)
{
    std::thread threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threads[i] = std::thread(elementDistributionMutex, i,std::ref(result));
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }
}

void sumWithAtomic(int value, std::atomic<int>& result)
{
    int old_value, new_value;

    if(value % 2 == 0)
    {
        do
        {
            old_value = result.load();
            new_value = old_value ^ value;
        }while(!result.compare_exchange_weak(old_value, new_value));
    }
}

void elementDistributionAtomic(const int thread_id, std::atomic<int>& result)
{
    for(int i = thread_id; i < SIZE; i+= NUM_THREADS)
    {
        sumWithAtomic(array[i], result);
    }
}

void calculateAtomicXOR(std::atomic<int>& result)
{
    std::thread threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threads[i] = std::thread(elementDistributionAtomic, i, std::ref(result));
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }
}

int main()
{
    srand(time(NULL));

    int sequantial_xor_result = 0;
    int synchronized_xor_result = 0;
    std::atomic<int> atomic_xor_result(0);

    fillArray();
    //printArray();

    calculateSequantialXOR(sequantial_xor_result);
    calculateSynchronizedXOR(synchronized_xor_result);
    calculateAtomicXOR(atomic_xor_result);

    std::cout << sequantial_xor_result << std::endl;
    std::cout << synchronized_xor_result << std::endl;
    std::cout << atomic_xor_result << std::endl;

    return 0;
}


