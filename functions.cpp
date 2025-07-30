#include <iostream>
#include <string>
using namespace std;

void display(const void* data, char type, size_t t = 0)
{
    switch (type)
    {
    case 'i':
    {
        cout << "Integer value: " << *(int*) data << endl;
        break;
    }
    
    case 'd':
        cout << "Double value: " << *(double*) data << endl;
        break;
    
    case 'a':
    { //C++ does not allow skipping over variable initialisations inside a switch stmt due to potential undefined behaviour from non-trivial constructors/desctructors.
        int j = 0;
        char* arr = (char*) data;
        while (j < t)
        {
            cout << *(arr + j);
            j++;
        }

        cout << endl;
        break;
    }
    
    default:
        cout << "Invalid option!!! Try again." << endl;
        break;
    }
}

inline int min(int x, int y) { return (x < y ? x : y); }

int add(void* data)
{
    int x = *(int*) data;
    return x * x;
}

int main() 
{
    int integer = 10;
    const double PI = 3.14159E30;

    char name_chars[3] = {'D', 'o', 'e'};

    void (*printer) (const void*, char, size_t); // ref to void is not allowed

    printer = display;
    printer(&integer, 'i', 0);
    printer(&PI, 'd', 0);
    printer(name_chars, 'a', 3);

    cout << "minimum: " << min(32, 21) << endl;
    
    int x = 21;
    cout << "square: " << add(&x) << endl;

    return 0;

}