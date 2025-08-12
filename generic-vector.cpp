#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

template <class T>
class Vectr
{
    protected:
        T* vectr;
        int index = 0;
        int capacity = 4;

    public:
        Vectr()
        : vectr(new T[capacity]())
        {}

        Vectr(const Vectr<T>& other)
        : index(other.index), capacity(other.capacity), vectr(new T[capacity]())
        {
            copy(other.vectr, other.vectr + other.index, vectr);
        }

        Vectr& operator=(const Vectr<T>& other)
        {
            if (this != &other)
            {
                delete[] vectr;
                index = other.index;
                capacity = other.capacity;
                vectr = new T[capacity]();
                for (int i = 0; i < index; i++)
                {
                    *(vectr + i) = other.vectr[i];
                }

            }

            return *this;
        }

        void push(T item)
        {
            if (index >= capacity)
            {
                capacity = index + 4;
                T* copy = new T[capacity]();
                for (int j = 0; j < index; j++)
                {
                    copy[j] = vectr[j];
                }
                delete[] vectr;
                vectr = copy;
                vectr[index] = item;
            }
            else
            {
                vectr[index] = item;
            }
            index ++;
        }

        template <class U>
        friend ostream& operator<< (ostream& out, const Vectr<U>& vector);

        virtual ~Vectr()
        {
            delete[] vectr;
            cout << "Vectr Destructor called" << endl;
        }
};

template <typename U>
ostream& operator<< (ostream& out, const Vectr<U>& vector)
{
    for (int i = 0; i < vector.index; i ++)
    {
        out << vector.vectr[i] << endl;
    }
    return out;
}

template <typename S>
class Stack : virtual public Vectr<S>
{
    public:
        Stack()
        : Vectr<S>()
        {}

        Stack(const Stack<S>& other)
        : Vectr<S>(other)
        {}

        Stack<S>& operator=(const Stack<S>& other)
        {
            Vectr<S>::operator=(other);
            return *this;
        }

        void pop()
        {
            this->vectr[(this->index) - 1] = '\0';
            (this->index)--;
        }

        ~Stack()
        {
            cout << "Stack Destructor called" << endl;
        }
};

template <typename Q>
class Queue : virtual public Vectr<Q>
{
    public:
        Queue()
        :Vectr<Q>()
        {
            cout << "constructor called" << endl;
        }

        Queue(const Queue<Q>& other)
        : Vectr<Q>(other)
        {}

        Queue& operator=(const Queue<Q>& other)
        {
            Vectr<Q>::operator=(other);
            return *this;
        }

        void popleft()
        {
            if ((this -> index) >= 1)
            {
                for (int i = 0; i < ((this -> index) - 1); i++)
                {
                    this->vectr[i] = this->vectr[i + 1];
                }
                this->index --;
            }
        }

        ~Queue()
        {
            cout << "Queue Destructor called" << endl;
        }
};

template <typename D>
class Dequeue : public Queue<D>, public Stack<D>
{
    public:
        Dequeue()
        : Queue<D>()
        {}

        Dequeue(const Dequeue<D>& other)
        : Queue<D>(other)
        {}

        Dequeue& operator=(const Dequeue<D>& other)
        {
            Queue<D>::operator=(other);
            return *this;
        }

        void pushleft(D item)
        {
            if (this->index >= this->capacity)
            {
                this->capacity = this->index + 4;
                D* copy = new D[this->capacity]();

                for (int i = this->index; i >= 1; i--)
                {
                    copy[i] = this->vectr[i - 1];
                }
                delete[] this->vectr;
                *(copy) = item;
                this->vectr = copy;
            }
            else
            {
                for (int i = this->index; i >= 1; i--)
                {
                    this->vectr[i] = this->vectr[i - 1];
                }
                *(this->vectr) = item;
            }
            this->index++;
        }

        ~Dequeue()
        {
            cout << "Dequeue Destructor called" << endl;
        }
};


int main()
{
    Queue<char> x ;
    x.push('a');
    x.push('b');
    x.push('c');
    x.push('d');
    x.push('e');
    x.push('f');
    
    Queue<char> y;
    y.push('b');
    y = x;
    cout << x << endl << y;

    // Queue<int>* nums = new Queue<int>;
    // nums->push(1);
    // nums->push(2);
    // nums->push(3);
    // nums->push(4);
    // nums->push(5);

    // Queue<int>* nums2 = nums;
    // nums2->popleft();
    // cout << *nums2 << endl;

    return 0;
}