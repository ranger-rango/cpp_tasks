#include <iostream>
#include <string>
#include <dlfcn.h>

using namespace std;

extern "C" 
{
    int add_int(int a, int b);
    float add_float(float a, float b);
    int sub_int(int a, int b);
    float sub_float(float a, float b);
    int mult_int(int a, int b);
    float mult_float(float a, float b);
    int div_int(int a, int b);
    float div_float(float a, float b);
}

int main()
{
    int a;
    int b;
    float x;
    float y;
    string choice;
    
    while (true)
    {
        cout << "MathLib >> ";
        getline(cin, choice);
        if (choice == "quit")
        {
            cout << "Goodbye :)" << endl;
            break;
        }

        cout << "Enter int a >> ";
        cin >> a ;

        cout << "Enter int b >> ";
        cin >> b ;

        cout << "Enter float x >> ";
        cin >> x ;

        cout << "Enter float y >> ";
        cin >> y ;
        cin.ignore();

        cout << "Math Operations: " << endl;
        cout << "\ta + b = " << add_int(a, b) << endl;
        cout << "\tx + y = " << add_float(x, y) << endl << endl;
        cout << "\ta - b = " << sub_int(a, b) << endl;
        cout << "\tx - y = " << sub_float(x, y) << endl << endl;
        cout << "\ta * b = " << mult_int(a, b) << endl;
        cout << "\tx * y = " << mult_float(x, y) << endl << endl;
        cout << "\ta / b = " << div_int(a, b) << endl;
        cout << "\tx / y = " << div_float(x, y) << endl;

    }

    void* handle = dlopen("./libmath_utils.so", RTLD_LAZY);
    if (!handle)
    {
        cerr << "Cannot open library: " << dlerror() << endl;
        return 1;
    }

    typedef double (*func_t)(double, double);
    func_t func = (func_t) dlsym(handle, "_Z3addIdET_S0_S0_");

    if (!func)
    {
        cerr << "Cannot load symbol: " << dlerror() << endl;
        return 1;
    }

    cout << "result: " << func(2.1, 4.3) << endl;

    dlclose(handle);

    return 0;
}