#include <iostream>
#include <string>
using namespace std;

template <typename T>
struct NodeS
{
    T data;
    NodeS<T>* next_node = nullptr;
};

template <typename T>
class NodeC
{
    public:
        T data;
        NodeC<T>* next_node = nullptr;
};

template <typename T>
class LinkedList
{
    private:
        T* head = nullptr;
    public:
        LinkedList()
        {}

        void append(const T& node)
        {
            T* newNode = new T(node);
            if (head == nullptr)
            {
                head = newNode;
            }
            else
            {
                T* currentNode = head;
                while (currentNode->next_node != nullptr)
                {
                    currentNode = currentNode->next_node;
                }
                currentNode->next_node = newNode;
            }

            newNode = nullptr;
        }

        void appendLeft(const T& node)
        {
            T* newNode = new T(node);
            newNode -> next_node = head;
            head = newNode;
            newNode = nullptr;
        }

        void pop()
        {
            T* currenNode = head;
            while (currenNode->next_node->next_node != nullptr)
            {
                currenNode = currenNode->next_node;
            }
            delete currenNode->next_node;
            currenNode->next_node = nullptr;
        }

        void popleft()
        {
            T* currentNode = head;
            head = currentNode -> next_node;
            delete currentNode;
            currentNode = nullptr;
        }

        void popByIndex(int index)
        {
            int indexCounter = 0;
            T* currentNode = head;
            while (currentNode != nullptr)
            {
                if (indexCounter == (index - 1))
                {
                    break;
                }
                currentNode = currentNode -> next_node;
                indexCounter++;
            }
            T* deleteNode = currentNode -> next_node;
            currentNode->next_node = deleteNode -> next_node;
            deleteNode = nullptr;
        }

        template <typename U>
        void popByData(U data)
        {
            T* currentNode = head;
            while (currentNode != nullptr)
            {
                if (currentNode->next_node->data == data)
                {
                    break;
                }
                currentNode = currentNode -> next_node;
            }
            T* deleteNode = currentNode -> next_node;
            currentNode->next_node = deleteNode -> next_node;
            deleteNode = nullptr;
        }

        void reverse()
        {
            if (head == nullptr || head->next_node == nullptr)
            {
                return;
            }

            T* prev = nullptr;
            T* currentNode = head;
            T* next = nullptr;

            while (currentNode != nullptr)
            {
                next = currentNode -> next_node;
                currentNode -> next_node = prev;
                prev = currentNode;
                currentNode = next;

            }
            head = prev;
        }

        T* getHead()
        {
            return head;
        }

        template <typename U>
        friend ostream& operator<< (ostream& out, const LinkedList<U>& linkedList);

        ~LinkedList()
        {}
};

template <typename T>
ostream& operator<< (ostream& out, const LinkedList<T>& linkedList)
{
    T* currentNode = linkedList.head;

    while (currentNode != nullptr)
    {
        out << currentNode->data << " -> ";
        currentNode = currentNode -> next_node;
    }
    out << endl;
    return out;
}

int main()
{
    NodeS<int> headS;
    headS.data = 10;

    NodeS<int> node1;
    node1.data = 20;

    NodeS<int> node2;
    node2.data = 30;

    NodeS<int> node3;
    node3.data = 40;

    NodeS<int> node4;
    node4.data = 0;

    LinkedList<NodeS<int>> llS;
    llS.append(headS);
    llS.append(node1);
    llS.append(node2);
    llS.append(node3);

    llS.appendLeft(node4);
    cout << llS << endl;
    llS.popByData(20);
    cout << llS << endl;

    cout << endl;

    NodeC<string> headC;
    headC.data = "The";

    NodeC<string> nodeC1;
    nodeC1.data = "quick";

    NodeC<string> nodeC2;
    nodeC2.data = "brown";

    NodeC<string> nodeC3;
    nodeC3.data = "fox";

    LinkedList<NodeC<string> > llC;
    llC.append(headC);
    llC.append(nodeC1);
    llC.append(nodeC2);
    llC.append(nodeC3);

    llC.popleft();
    cout << llC << endl;
    llC.reverse();
    cout << llC << endl;

    return 0;
}