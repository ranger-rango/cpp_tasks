#include <iostream>
#include <memory>
using namespace std;

int main ()
{

    // datatypes basics 
    int numeric = 256;
    int octal = 0400;
    int hex = 0x100;
    cout << "numeric: " << numeric << ", size: " << sizeof(numeric) <<"\nocatal: " << octal << ", size: " << sizeof(octal) << "\nhex: " << hex << ", size: " << sizeof(hex) << endl;
    cout << endl;
    
    long big_number = 314159265L;
    cout << "big number (long): " << big_number << ", size: " << sizeof(big_number) << endl;
    cout << endl;

    float floating_point_pi = 3.14159F;
    double literal_float = 3.14159E23;
    cout << "floating point pi float: " << floating_point_pi << ", size: " << sizeof(floating_point_pi) << endl;
    cout << "literal float: " << literal_float << ", size: " << sizeof(literal_float) << endl;
    cout << endl;

    char x = 'X';
    cout << "x : " << x << ", size: " << sizeof(x) << endl;
    cout << endl;

    enum Day {SUN, MON, TUE};
    enum Day1 {WED = 1, THUR = 2, FRI = 3};
    Day today = TUE;
    Day1 tomorrow = WED;
    cout << "today: " << today << ", size: " << sizeof(today) << endl;
    cout << "tomorrow: " << tomorrow << ", size: " << sizeof(tomorrow) << endl;
    cout << endl;

    // ptr, array basics 
    int number = 9;
    int* ptr;
    ptr =  &number;
    *ptr = 3;

    cout <<"memaddr of number: " << &number << ", value of number: " << *ptr << endl;
    cout << endl;


    int numbers[7] = {32, 43, 56, 82, 47, 93, 21};
    int* ptrs_to_numbers[7];

    int arr_length = sizeof(numbers) / sizeof(numbers[0]);
    
    int i;
    for (i = 0 ; i < arr_length; i++)
    {
        ptrs_to_numbers[i] = &numbers[i];

        if ((numbers[i] / 2 ) == 0)
        {
            *ptrs_to_numbers[i] = *ptrs_to_numbers[i] / 2;
        }
        else if ((numbers[i] / 2 ) > 0)
        {
            *ptrs_to_numbers[i] = *ptrs_to_numbers[i] * 2;
        }
        else
        {
            continue;
        }
    }

    bool flag = true;
    int j = 0;
    while (flag)
    {
        cout << " " << *(numbers + j) << " ";
        j++;

        if (j == arr_length)
        {
            flag = false;
        }
    }

    cout << endl;
    cout << endl;

    // pointer-array relationship 
    char character[] = {'c', 'a', 't'};
    char* ptr_to_char = character;
    char* ptr_to_char1 = &character[0];
    cout << character[2] << ptr_to_char[2] << ptr_to_char1[2] << endl;
    cout << endl;

    // heap and stack
    char* dynamic_arr = new char[5]();
    dynamic_arr[3] = 'd';
    *(dynamic_arr + 2) = 'b';
    int k = 0;
    while (k < 5)
    {
        cout << " " << dynamic_arr[k] << " ";
        k++;
    }
    cout << endl;
    cout << endl;
    
    delete[] dynamic_arr;
    dynamic_arr = nullptr;


    // 2's complements mechanics for switching to -ve 
    int max = 2147483647;
    cout << max + 2 << endl;
    cout << endl;

    // smart pointers

    // owns the object exclusively, can't be copied, auto deletes object when it goes out of scope.
    unique_ptr<int> smart_int_ptr = make_unique<int>(42);

    // multiple ptrs share ownership of the same object 
    // keeps a ref count internally 
    // deletes the obj when the last shared_ptr is destroyed 

    shared_ptr<int> smart_int_ptr1 = make_shared<int>(10);
    shared_ptr<int> smart_int_ptr2 = smart_int_ptr1;
    weak_ptr<int> wp = smart_int_ptr1;
        // doesn't increase ref count.
        // breaks cyclic references.

    return 0;
}