#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <queue>
using namespace std;

void printNumbers(int from, int to)
{
    for (int i = from; i <= to; i++)
    {
        cout << i << " ";
    }
    cout << endl;
}

int counter = 0;
mutex mtx;
// blocking
void increment(int n)
{
    for (int i = 0; i < n; i++)
    {
        lock_guard<mutex> lock(mtx);
        counter++;
    }
}

// conditional variables 
queue<int> q;
mutex mtxx;
condition_variable cv;

void producer()
{
    for (int i = 0; i <= 5; i++)
    {
        unique_lock<mutex> lock(mtxx);
        q.push(i);
        cout << "Produced: " << i << endl;
        cv.notify_one(); // notify one waiting thread
    }
}

void consumer()
{
    for (int i = 0; i <= 5; i++)
    {
        unique_lock<mutex> lock(mtxx);
        cv.wait(lock, [](){return !q.empty(); });
        int item = q.front();
        q.pop();
        cout << "Consumed: " << item << endl;
    }
}

int main()
{
    thread printNumst1(printNumbers, 1, 5);
    thread printNumst2(printNumbers, 6, 10);
    printNumst1.join();
    printNumst2.join();
    cout << "Both threads have completed execution" << endl << endl;

    thread inc1(increment, 1000);
    thread inc2(increment, 1000);
    inc1.join();
    inc2.join();
    cout << "Counter: " << counter << endl << endl;

    thread prod(producer);
    thread consm(consumer);

    prod.join();
    consm.join();

    auto slowAdd = [](int a, int b) -> int
    {
        this_thread::sleep_for(chrono::seconds(2));
        return a + b;
    };

    future<int> result = async(launch::async, slowAdd, 5, 7);
    cout << endl << "Doing other work while slowAdd runs ..." << endl;
    int sum = result.get();
    cout << "Result: " << sum << endl << endl;
    
    return 0;
}