#include <iostream>
#include <string>
using namespace std;

template <class T>
class Vectr
{
    // private:
    protected:
        T* vectr;
        int index = 0;
        int capacity = 4;

    public:
        Vectr()
        : vectr(new T[capacity]())
        {}

        Vectr(const Vectr<T>& other)
        : index(other.index), vectr(new T[index]())
        {
            copy(other.vectr, other.vectr + index, vectr);
        }

        Vectr& operator=(const Vectr<T>& other)
        {
            if (this != &other)
            {
                delete[] vectr;
                index = other.index;
                vectr = new T[index]();
                for (int i = 0; i < index; i++)
                {
                    *(vectr + i) = other.vectr[i];
                }

            }

            return this;
        }

        virtual void push(T item)
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
                copy[index] = item;
                vectr = copy;
            }
            else
            {
                vectr[index] = item;
            }
            index ++;
        }

        template <class U>
        friend ostream& operator<< (ostream& out, const Vectr<U>& vector);

        ~Vectr()
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
class Stack : public Vectr<S>
{
    public:
        Stack()
        : Vectr<S>()
        {}

        Stack(const Stack<S>& other)
        : Vectr<S>(other)
        {}

        Stack<S>& operator=(const Stack<S> other)
        {
            Vectr<S>::operator=(other);
            return *this;
        }

        void push(S item)
        {
            Vectr<S>::push(item);
        }

        void pop()
        {
            if ((this -> index) >= (this -> capacity))
            {
                this -> vectr[(this -> index) - 1] = '\0';
            }
            else
            {
                this -> vectr[(this -> index) - 1] = 0;
            }
            (this -> index)--;
        }

        ~Stack()
        {
            cout << "Stack Destructor called" << endl;
        }
};

template <typename Q>
class Queue : public Vectr<Q>
{
    public:
        Queue()
        :Vectr<Q>()
        {}

        Queue(const Queue<Q>& other)
        : Vectr<Q>(other)
        {}

        Queue& operator=(const Queue<Q>& other)
        {
            Vectr<Q>::operator=(other);
        }

        // void push(Q item)
        // {
        //     Vectr<Q>::push(item);
        // }

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


int main()
{
    // Vectr<char> x;
    // x.push('a');
    // x.push('b');
    // x.push('c');
    // x.push('d');
    // x.push('e');

    // cout << x << endl;
    // cout << x << endl;

    // Stack<int> stk;
    // stk.push(1);
    // stk.push(2);
    // stk.push(3);
    // cout << stk << endl;
    // stk.pop();
    // cout << stk << endl;

    Vectr<char>* queue = new Queue<char>;
    queue->push('a');
    queue->push('v');
    queue->push('d');
    queue->push('t');
    cout << *queue << endl;
    dynamic_cast< Queue<char>* > (queue)->popleft();
    cout << *queue << endl;

    cout << endl;
    Queue<char> queue1 = *dynamic_cast<Queue<char>*> (queue);
    queue1.push('b');
    queue1.popleft();
    cout << queue1 << endl << endl << *queue << endl;

    return 0;
}