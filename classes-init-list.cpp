#include <iostream>
#include <string>
using namespace std;

class Engine
{
    int horsepower;

    public:
        Engine(int hp) : horsepower(hp)
        {
            cout << "Engine constructed with: " << horsepower << endl;
        }
};

class Car
{
    Engine engine;
    string carModel;

    public:
        Car(int hp, string model) : engine(hp), carModel(model)
        {
            cout << "Car Constructed: " << carModel << endl;
        }
};

int main()
{
    Car myCar(500, "Lexus LC500");
    return 0;
}