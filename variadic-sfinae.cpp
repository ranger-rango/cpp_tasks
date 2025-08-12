#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
using namespace std;

template <typename T>
typename enable_if<is_integral<T>::value>::type
foo(T)
{
    cout << "Integral type" << endl;
}

template <typename T>
typename enable_if<is_floating_point<T>::value>::type
foo(T)
{
    cout << "Floating point type" << endl;
}

template <typename... Args>
void print(Args&&... args)
{
    (cout << ... << args ) << endl;
    // ((cout << args), ...);
    // cout << endl;
    // (void)initializer_list<int>{ (cout << args << ' ', 0)... };
    // cout << endl;
}

int main()
{
    foo(42);
    foo(42.221);

    print(1, " ", "foo", " ", 4.5322f, " ", 3.14159E67, " ", 'd');

    return 0;
}