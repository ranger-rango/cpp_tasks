#include <iostream>
#include <string>
using namespace std;

enum BodyPlan {QUADRUPED, BIPED};
enum Reproduction {PLACENTAL, MONOTREME};
enum Diet {CARNIVORE, HERBIVORE, OMNIVORE};

class Mammalia
{
    private:
        BodyPlan bodyPlan = QUADRUPED;
        Reproduction reproduction = MONOTREME;
        string thermal = "HomeoTherm";
    public:
        Mammalia() {}

        Mammalia(BodyPlan body_plan, Reproduction reprod)
        : bodyPlan(body_plan), reproduction(reprod)
        {}

        void setBodyPlan(BodyPlan body_plan)
        {
            bodyPlan = body_plan;
        }

        void setReproduction(Reproduction reprod)
        {
            reproduction = reprod;
        }

        Reproduction getReproduction() const
        {
            return reproduction;
        }

        BodyPlan getBodyPlan() const
        {
            return bodyPlan;
        }

        ~Mammalia()
        {
            cout << "Mammalia Destructor called." << endl;
        }
};

class Masurpialia
{
    private:
        string uAtrribute = "Masurpial";
    public:
        Masurpialia() {}

        string getUAttribute() const
        {
            return uAtrribute;
        }

        ~Masurpialia()
        {
            cout << "Masurpial destructor called" << endl;
        }
};

class Kangaroo : public Mammalia, public Masurpialia
{
    private:
        Diet kangarooDiet = HERBIVORE;
    public:
        Kangaroo() {}

        Diet getDiet() const
        {
            return kangarooDiet;
        }

        friend ostream& operator<< (ostream& out, const Kangaroo& kangaroo);

        ~Kangaroo()
        {
            cout << "Kangaroo Destructor called" << endl;
        }
};

ostream& operator<< (ostream& out, const Kangaroo* kangaroo)
{
    out << "Pointer:\n\nKangaroo:" << "\n\tBodyPlan: " << kangaroo -> getBodyPlan() << "\n\tReproduction: " << kangaroo -> getReproduction() << "\n\tOrder: " << kangaroo -> getUAttribute() << "\n\tDiet: " << kangaroo -> getDiet() << endl;
    return out;
}

ostream& operator<< (ostream& out, const Kangaroo& kangaroo)
{
    out << "Reference:\n\nKangaroo:" << "\n\tBodyPlan: " << kangaroo.getBodyPlan() << "\n\tReproduction: " << kangaroo.getReproduction() << "\n\tOrder: " << kangaroo.getUAttribute() << "\n\tDiet: " << kangaroo.kangarooDiet << endl;
    return out;
}

int main()
{
    Kangaroo* kangaroo = new Kangaroo;
    BodyPlan kangaroobp = BIPED;
    Reproduction kangaroorep = MONOTREME;
    kangaroo -> setBodyPlan(kangaroobp);
    kangaroo -> setReproduction(kangaroorep);

    Kangaroo& ref = *kangaroo;
    cout << kangaroo << endl;
    cout << ref << endl;
    delete kangaroo;
    return 0;
}