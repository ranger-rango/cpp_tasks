#pragma once


template <typename T>
class Operand
{
    private:
        T opX = 0;
    
    public:
        Operand(T num1)
        : opX(num1)
        {}

        T operator+ (const Operand& other) const
        {
            return opX + other.opX;
        }

        T operator- (const Operand& other) const
        {
            return opX - other.opX;
        }

        T operator* (const Operand& other) const
        {
            return opX * other.opX;
        }

        T operator/ (const Operand& other) const
        {
            return opX / other.opX;
        }

        ~Operand()
        {}
};