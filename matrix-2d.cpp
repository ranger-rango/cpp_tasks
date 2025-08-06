#include <iostream>
using namespace std;

class MatrixTwoD
{
    private:
        int matrixRows = 0;
        int matrixCols = 0;
        int** matrix = nullptr;

    public:
        MatrixTwoD() {}

        MatrixTwoD(int rows, int cols)
        : matrixRows(rows), matrixCols(cols)
        {
            matrix = new int*[matrixRows]();
            for (int i = 0; i < matrixRows; i++)
            {
                *(matrix + i) = new int[matrixCols]();
            }
        }

        void fillMatrix(int fillNum)
        {
            for (int i = 0; i < matrixRows; i++)
            {
                for (int j = 0; j < matrixCols; j++)
                {
                    *(*(matrix + i) + j) = fillNum;
                }
            }
        }

        ~MatrixTwoD()
        {
            for (int j = 0; j < matrixRows; j++)
            {
                delete[] matrix[j];
                matrix[j] = nullptr;
            }
            delete[] matrix;
            matrix = nullptr;    
        }

        friend ostream& operator<< (ostream& out, const MatrixTwoD& matrixTwoD);
};

ostream& operator<< (ostream& out, const MatrixTwoD& matrixTwoD)
{
    for (int i = 0; i < matrixTwoD.matrixRows; i++)
    {
        for (int j = 0; j < matrixTwoD.matrixCols; j++)
        {
            out << "[" << *(*(matrixTwoD.matrix + i) + j) << "]";
        }
        out << endl;
    }
    
    return out;
}

int main()
{
    MatrixTwoD* m = new MatrixTwoD(3, 2);
    m -> fillMatrix(8);
    cout << *m << endl;
    return 0;
}