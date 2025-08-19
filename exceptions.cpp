#include <iostream>
#include <string>
#include <functional>
// #include <scope>
using namespace std;

class MathException
{
    private:
        string errMsg;
    public:
        MathException(const string& err)
        : errMsg(err)
        {}

        string getErrMsg()
        {
            return errMsg;
        }
};

class ZeroDivision : public MathException
{
    public:
        ZeroDivision(const string& err)
        : MathException(err)
        {}
};

void calculator(int x, int y) // can throw anything.
{
    if (x == 0)
    {
        throw ZeroDivision("Math Exception: ZeroDivision");
    }
    cout << (float) y / x <<endl;
}
// void calculator(int x, int y) noexcept; Guarantees no exceptions 

class Finally
{
    public:
        function<void()> func;
        Finally(function<void()> f)
        :   func(move(f))
        {}
        ~Finally()
        {
            func();
        }
};

int main()
{
    try
    {
        // scope_exit cleanup{ ...} ;
        Finally cleanup([]
        {
            cout << "This is the finally block" << endl;
        });

        calculator(0, 34);

    }
    catch(ZeroDivision& e)
    {
        cerr << e.getErrMsg() << endl;
    }
    
    return 0;
}