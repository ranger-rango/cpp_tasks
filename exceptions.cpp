#include <iostream>
#include <string>
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

int main()
{
    try
    {
        calculator(6, 34);

    }
    catch(ZeroDivision& e)
    {
        cerr << e.getErrMsg() << endl;
    }
    
    return 0;
}