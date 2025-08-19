#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

class BankAccount
{
    private:
        int balance = 0;
        mutex mtx;
        condition_variable cv;
    
        public:
            BankAccount(int initialBalance)
            : balance(initialBalance)
            {}

            void deposit(int amount)
            {
                lock_guard<mutex> lock(mtx);
                balance += amount;
                cout << "[Deposit] New Balance: " << balance << endl;
                cv.notify_all();
            }

            int getBalance()
            {
                lock_guard<mutex> lock(mtx);
                return balance;
            }

            void withdraw(int amount)
            {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this, amount](){return balance >= amount; });
                balance -= amount;
                cout << "[Withdraw] New Balance: " << balance << endl;
            }

            ~BankAccount()
            {}
};



int main()
{
    BankAccount account(100);
    vector<thread> threads;

    threads.emplace_back(&BankAccount::deposit, ref(account), 100); 

    thread withdraw(&BankAccount::withdraw, ref(account), 300);
    threads.push_back(move(withdraw));

    threads.emplace_back(&BankAccount::deposit, ref(account), 100);

    for (auto &t : threads)
    {
        t.join();
    }

    cout << "Final Balance: " << account.getBalance() << endl;
    return 0;
}