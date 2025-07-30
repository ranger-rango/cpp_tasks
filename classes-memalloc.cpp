#include <iostream>
#include <algorithm>
using namespace std;

class Vect
{
    int n;
    char* data;

    public:
        Vect(int size);
        Vect(const Vect& other);
        Vect& operator=(const Vect& other);
        ~Vect()
        {
            delete[] data;
            data = nullptr;
            cout << "Destructed" << endl;
        }

        void setVectorElement(int index, char letter)
        {
            data[index] = letter;
        }

        char* getData()
        {
            return data;
        }
};

Vect::Vect(int size)
: n(size)
{
    data = new char[n]();
}

Vect::Vect(const Vect& other)
: n(other.n), data(new char[n]())
{
    copy(other.data, (other.data + n), data);
}

Vect& Vect::operator=(const Vect& other)
{
    if (this != &other)
    {
        delete[] data;
        n = other.n;
        data = new char[n]();
        copy(other.data, (other.data + n), data);
    }

    return *this;
}

int main()
{
    Vect chars(3);
    chars.setVectorElement(0, 'a');
    chars.setVectorElement(1, 'b');
    chars.setVectorElement(2, 'c');

    char* vectData = chars.getData();
    int i;
    for (i = 0; i < 3; i++)
    {
        cout << *(vectData + i) << " ";
    }
    cout << endl;

    return 0;
}