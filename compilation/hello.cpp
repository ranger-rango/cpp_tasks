#include <iostream>
using namespace std;

extern "C" void sayHello()
{
    cout << "Hello from Cross compiled lib" << endl;
    return;
}