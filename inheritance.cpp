#include <iostream>
#include <string>
using namespace std;

class Mammalia
{
    public:
        virtual void order() = 0;
        virtual void isBiped() = 0;
        virtual void describeMammal() const = 0;
        virtual ~Mammalia() {}
};

class Kangaroo : public Mammalia
{
    private:
        string kangarooOrder = "";
        bool isbiped = false;

    public:
        Kangaroo()
        {
            order();
            isBiped();
        }
        virtual void order()
        {
            kangarooOrder = "Marsupialia";
        }

        virtual void isBiped()
        {
            isbiped = true;
        }

        virtual void describeMammal() const
        {
            cout << "Name: Kangaroo" << "\nOrder: " << kangarooOrder << "\nIs Biped: " << isbiped << endl;
        }

        virtual ~Kangaroo()
        {
            cout << "Virtual Kangaroo destructor called" << endl;
        }

};

class Dog
{
    private:
        int dogDrink = 0;
        int dogPants = 1;

    public:
        Dog() {}
        Dog(int drink, int pants)
        :dogDrink(drink), dogPants(pants)
        {}
        void bark()
        {
            cout << "woof woof" << endl;
        }
        virtual void growl()
        {
            cout << "grrr grrrr" << endl;
        }
        virtual ~Dog()
        {
            cout << "Virtual Dog Destructor called" << endl;
        }
};

class Basenji : public Dog
{
    private:
        int dogSpeed;
    public:
        Basenji() {}
        Basenji(int drink, int pants, int speed)
        : Dog(drink, pants), dogSpeed(speed)
        {}
        void hunt()
        {
            cout << "Catching small game" << endl;
        }

        void bark()
        {
            cout << "woof yoddle woof yoddle" << endl;
        }
        virtual void growl()
        {
            cout << "silent" << endl;
        }
        ~Basenji()
        {
            cout << "Basenji destructor called." << endl;
        }
};

int main()
{
    Dog* basenji = new Basenji(1, 1, 7);
    // Static Binding
    basenji -> bark();
    dynamic_cast<Basenji*> (basenji) -> bark();

    basenji -> growl(); // Dynamic Binding

    // Class casting 
    // basenji -> hunt(); // Class "Dog" has no member "hunt"
    dynamic_cast<Basenji*> (basenji) -> hunt();

    delete basenji;

    cout << endl;

    Mammalia* kangaroo = new Kangaroo;
    kangaroo -> describeMammal();

    delete kangaroo;

    Kangaroo* kangaroo1 = dynamic_cast<Kangaroo*> (kangaroo);


    return 0;
}