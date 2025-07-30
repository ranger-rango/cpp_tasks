#include <iostream>
using namespace std;

class StringBuilder
{
    int index = 0;
    char* characters;

    public:
        StringBuilder()
        : characters(new char[4]())
        {}

        StringBuilder(const StringBuilder& other)
        : characters(new char[other.index]())
        {
            int i ;
            for (i = 0; i <= other.index; i++)
            {
                *(characters + i) = *(other.characters + i);
            }
        }

        StringBuilder& operator=(const StringBuilder& other)
        {
            if (this != &other)
            {
                delete[] characters;
                characters = new char[other.index]();
                int i;
                for (i = 0; i <= other.index; i++)
                {
                    characters[i] = other.characters[i]; 
                }
            }

            return *this;
        }

        void append(char character)
        {
            if (index == 0)
            {
                characters[index] = character;
            }
            if (characters[index] != '\0')
            {
                char* new_characters = new char[index + 4]();
                int i;
                for (i = 0; i <= index; i++)
                {
                    new_characters[i] = characters[i];
                }

                new_characters[index] = character;
                delete[] characters;
                characters = new_characters;
            }
            else
            {
                characters[index] = character;
            }
            index ++;

        }

        friend ostream& operator<<(ostream& out, const StringBuilder& builder);

        ~StringBuilder()
        {
            delete[] characters;
            characters = nullptr;
            cout << "Builder Object Destroyed." << endl;
        }
};

ostream& operator<<(ostream& out, const StringBuilder& builder)
{
    int i;
    for (i = 0; i <= builder.index; i++)
    {
        out << *(builder.characters + i);
    }

    return out;
}

int main()
{

    StringBuilder builder;
    builder.append('c');
    builder.append('a');
    builder.append('t');
    builder.append('t');
    builder.append('z');

    cout << builder << endl;
    return 0;
}