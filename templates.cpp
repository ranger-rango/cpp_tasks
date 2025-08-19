#include <iostream>
#include <string>
#include <type_traits>
using namespace std;

template <typename T>
T genericMin(T a, T b)// -> decltype(a < b ? a : b)
{
    return (a < b ? a : b);
}

template <typename T>
class GenericNum
{
    private:
        T x;
    public:
        GenericNum() {}
        GenericNum(T a)
        : x(a)
        {}

        T getNum()
        {
            return x;
        }

        T operator+ (const GenericNum& y);
        T operator- (const GenericNum& y);
        ~GenericNum()
        {
            cout << "Destructor called" << endl;
        }
};

template <typename T>
T GenericNum<T>::operator+ (const GenericNum& y)
{
    return x + y.x;
}

template <typename T>
T GenericNum<T>::operator- (const GenericNum& y)
{
    return x - y.x;
}

int main()
{
    cout << genericMin<>(5, 2) << endl;
    cout << genericMin<>(2.1, 8.3) << endl;
    cout << genericMin<>('r', 'd') << endl;

    GenericNum<float>* x = new GenericNum<float> (1.3);
    GenericNum<float>* y = new GenericNum<float> (1.4);
    cout << endl << (x->getNum() + y->getNum()) << endl <<(x->getNum() - y->getNum()) << endl;

    return 0;
}