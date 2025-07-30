#include <iostream>
using namespace std;

int main()
{
    int matrix[5][5];

    int i;
    int count = 1;
    for (i = 0; i < 5; i++)
    {
        int j;

        for (j = 0; j < 5; j++)
        {
            *(*(matrix + i) + j) = count;
            count ++;
        }
    }

    for (i = 0; i < 5; i++)
    {
        int j;

        for (j = 0; j < 5; j++)
        {
            cout << "matrix[" << i << "][" << j << "] : " << *(*(matrix + i) + j) << endl;
        }
        
    }    

}