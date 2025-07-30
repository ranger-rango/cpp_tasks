#include <iostream>
#include <string>
using namespace std;

class Counter
{
    private:
        int count;
    
    public:
        Counter();
        int getCount();
        void increaseBy(int x);
};

Counter::Counter()
{
    count = 0;
}

int Counter::getCount()
{
    return count;
}

void Counter::increaseBy(int x)
{
    count += x;
}

enum MealType {REGULAR, NO_FAT, KETO};

class Passenger
{

    private:
        string name;
        MealType meal_pref;
        bool is_freq_flyer;
        string freq_flyer_num;

    public:
        Passenger()
        {
            name = "__no_name";
            meal_pref = REGULAR;
            is_freq_flyer = false;
            freq_flyer_num = "NONE";
        }
        Passenger (string passName, MealType passPref, string passFreqNo = "NONE")
        : name(passName), meal_pref(passPref), is_freq_flyer(passFreqNo != "NONE"), freq_flyer_num(passFreqNo)
        {}

        Passenger (const Passenger& pass)
        : name(pass.name), meal_pref(pass.meal_pref), is_freq_flyer(pass.is_freq_flyer), freq_flyer_num(pass.freq_flyer_num)
        {}

        bool isFrequentFlyer() const
        {
            return is_freq_flyer;
        }
        void makeFrequentFlyer(const string& newFreqFlyerNo)
        {
            is_freq_flyer = true;
            freq_flyer_num = newFreqFlyerNo;
        }

        string getName()
        {
            return name;
        }

        MealType getMealPref()
        {
            return meal_pref;
        }

        friend ostream& operator<< (ostream& out, const Passenger& passenger);


};

ostream& operator<< (ostream& out, const Passenger& passenger)
{
    out << "Passenger\n\tName: " << passenger.name << "\n\tMeal Prefference: " << passenger.meal_pref << "\n\tIs Frequent Flyer: " << passenger.is_freq_flyer << "\n\tFrequent Flyer Number: " << passenger.freq_flyer_num << endl;
    return out;
}

int main()
{
    Counter ctr;
    cout << ctr.getCount() << endl;
    ctr.increaseBy(2);
    cout << ctr.getCount() << endl;
    ctr.increaseBy(3);
    cout << ctr.getCount() << endl;

    Passenger* pass = nullptr;
    pass = new Passenger(); 
    cout << "name: " << pass->getName() << ", meal preference: " << pass->getMealPref() << endl; 
    cout << pass->isFrequentFlyer() << endl;
    pass->makeFrequentFlyer("32800395Q");
    cout << pass->isFrequentFlyer() << endl;
    cout << endl;

    delete pass;
    pass = nullptr;

    Passenger passenger("Jane Doe", NO_FAT);
    cout << endl;
    cout << passenger << endl;
    // cout << "name: " << passenger.getName() << ", meal preference: " << passenger.getMealPref() << ", is freq flyer: " << passenger.isFrequentFlyer() << endl; 
    // cout << passenger.isFrequentFlyer() << endl;
    passenger.makeFrequentFlyer("32800395Q");
    // cout << passenger.isFrequentFlyer() << endl;
    cout << endl;
    cout << passenger << endl;

    Passenger passenger1 = passenger;
    cout << endl;
    cout << passenger1 << endl;
    // cout << "name: " << passenger1.getName() << ", meal preference: " << passenger1.getMealPref() << ", is freq flyer: " << passenger1.isFrequentFlyer() << endl;

    Passenger passenger2(passenger1);
    cout << endl;
    cout << passenger2 << endl;
    // cout << "name: " << passenger2.getName() << ", meal preference: " << passenger2.getMealPref() << ", is freq flyer: " << passenger2.isFrequentFlyer() << endl;



    return 0;
}