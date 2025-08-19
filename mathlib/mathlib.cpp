#include "mathlib.hpp"

extern "C" 
{
    int add_int(int a, int b)
    {
        Operand<int> x(a);
        Operand<int> y(b);
        return x + y;
    }

    float add_float(float a, float b)
    {
        Operand<float> x(a);
        Operand<float> y(b);
        return x + y;
    }

    int sub_int(int a, int b)
    {
        Operand<int> x(a);
        Operand<int> y(b);
        return x - y;
    }

    float sub_float(float a, float b)
    {
        Operand<float> x(a);
        Operand<float> y(b);
        return x - y;
    }

    int mult_int(int a, int b)
    {
        Operand<int> x(a);
        Operand<int> y(b);
        return x * y;
    }

    float mult_float(float a, float b)
    {
        Operand<float> x(a);
        Operand<float> y(b);
        return x * y;
    }

    int div_int(int a, int b)
    {
        Operand<int> x(a);
        Operand<int> y(b);
        return x / y;
    }

    float div_float(float a, float b)
    {
        Operand<float> x(a);
        Operand<float> y(b);
        return x / y;
    }

}