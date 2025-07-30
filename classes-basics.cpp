#include <iostream>
using namespace std;

class Person
{
    int personAge;
    int personMarks;
    public:
        Person() {}
        Person(int age, int marks)
        : personAge(age), personMarks(marks)
        {}

        void setAge(int age)
        {
            personAge = age;
        }

        void setMarks(int marks)
        {
            personMarks = marks;
        }

        int getAge()
        {
            return personAge;
        }

        int getMarks()
        {
            return personMarks;
        }

        friend ostream& operator<< (ostream& out, const Person& person);
};

ostream& operator<< (ostream& out, const Person& person)
{
    out << "Person Age: " << person.personAge << ", Person Marks: " << person.personMarks << endl;
    return out;
}

void manipulatePerson(Person person)
{
    person.setAge(21);
    person.setMarks(78);
    cout << "In Pass By Value Function:\n\tAge: " << person.getAge() << ", Marks: " << person.getMarks() << endl;
    cout << endl;

}

void manipulatePerson(Person* person)
{
    person->setAge(21);
    person->setMarks(78);
    cout << "In Pass By Reference Function:\n\tAge: " << person->getAge() << ", Marks: " << person->getMarks() << endl;
    cout << endl;

}

void manipulatePerson2(Person& person)
{
    person.setAge(27);
    person.setMarks(65);
    cout << "In Pass By Reference_2 Function:\n\tAge: " << person.getAge() << ", Marks: " << person.getMarks() << endl;
    cout << endl;
}


int main()
{
    Person person(23, 89);
    cout << "In Main:\n\tAge: " << person.getAge() << ", Marks: " << person.getMarks() << endl;
    cout << endl;

    manipulatePerson(person);
    cout << "After Pass By Value Function:\n\tAge: " << person.getAge() << ", Marks: " << person.getMarks() << endl;
    cout << endl;

    manipulatePerson(&person);
    cout << "After Pass By Reference Function:\n\tAge: " << person.getAge() << ", Marks: " << person.getMarks() << endl;
    cout << endl;

    manipulatePerson2(person);
    cout << "After Pass By Reference_2 Function:\n\tAge: " << person.getAge() << ", Marks: " << person.getMarks() << endl;
    cout << endl;

    cout << "Testing << overload: " << endl;
    cout << person << endl;

    return 0;
}