#include <iostream>
#include <thread>
#include <stdexcept>
#include <thread>
#include <vector>
#include <future>
#include <string>
#include <mutex>
#include <atomic>

using namespace std;

// exception caught inside thread 
void worker(int id)
{
    try
    {
        if (id == 2)
        {
            throw runtime_error("Catastrophic failure occured.");
        }
        cout << "Worker: " << id << " continuing with task ..." << endl;
    }
    catch(const std::exception& e)
    {
        cerr << "Thread Exception: " << e.what() << '\n';
    }
    
}

// exception propergation to main thread 
mutex mtx;
int riskyTask(int id)
{
    
    if (id == 2)
    {
        lock_guard<mutex> lock(mtx);
        string errMsg = "RiskyTask Id " + to_string(id) + " collapsed.";
        throw runtime_error(errMsg);
    }
    {
        lock_guard<mutex> lock(mtx);
        cout << "RiskyTask Id: " << id << " run to completion" << endl;
    }
    return id * 10;
}

// thread control via exceptions 
atomic<bool> stopFlag(false);
void controlledThread(int id)
{
    try
    {
        for (int i = 0; i < 10; i++)
        {
            if (stopFlag)
            {
                throw runtime_error("Stop signal received");
            }
            cout << "Thread: " << id << " working on step " << i << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    catch(const std::exception& e)
    {
        cerr <<"Thread " << id << " stopped: " << e.what() << endl;
    }
    
}


int main()
{
    vector<thread> workerThreads;
    workerThreads.emplace_back(worker, 1);
    workerThreads.emplace_back(worker, 2);
    for (thread &t : workerThreads)
    {
        t.join();
    }

    cout << endl;
    future<int> risky1 = async(launch::async, riskyTask, 1);
    future<int> risky2 = async(launch::async, riskyTask, 2);
    future<int> risky3 = async(launch::async, riskyTask, 3);

    auto handleFuture = [](future<int>& f)
    {
        try
        {
            int value = f.get();
            lock_guard<mutex> lock(mtx);
            cout << "Results: " << value << endl;
        }
        catch(const std::exception& e)
        {
            lock_guard<mutex> lock(mtx);
            cerr << "RiskyTask Exception: " << e.what() << '\n';
        }
    };

    handleFuture(risky1);
    handleFuture(risky2);
    handleFuture(risky3);

    cout << endl;
    thread contThread(controlledThread, 1);
    this_thread::sleep_for(chrono::milliseconds(500));
    stopFlag = true;
    contThread.join();
    

    return 0;
}