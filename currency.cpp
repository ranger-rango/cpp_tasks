#include <iostream>
#include <string>
using namespace std;

class Currency
{
    private:
        string currencyName = "";
        float currencyAmount = 0.0f;
        float currencyConversionRate = 1.0f; // pegged on 1UNIT

    public:
        Currency() {}

        Currency(string name, float amount, float strength)
        : currencyName(name), currencyAmount(amount), currencyConversionRate(strength)
        {}

        Currency(const Currency& other)
        : currencyName(other.currencyName), currencyAmount(other.currencyAmount), currencyConversionRate(other.currencyConversionRate)
        {}

        Currency& operator=(const Currency& other)
        {
            currencyName = other.currencyName;
            currencyAmount = other.currencyAmount;
            currencyConversionRate = other.currencyConversionRate;

            return *this;
        }

        bool operator==(const Currency& other) const
        {
            return (currencyName == other.currencyName) && (currencyAmount == other.currencyAmount) && (currencyConversionRate == other.currencyConversionRate);
        }

        bool operator!=(const Currency& other) const
        {
            return (currencyName != other.currencyName) || (currencyAmount != other.currencyAmount) || (currencyConversionRate != other.currencyConversionRate);
        }

        bool operator>(const Currency& other) const
        {
            if (this != &other)
            {
                Currency temp = *this;
                temp >> other;
                return (temp.currencyAmount > other.currencyAmount);
            }
            return (currencyAmount > other.currencyAmount);
            
        }

        bool operator<(const Currency& other) const
        {
            if (this != &other)
            {
                Currency temp = *this;
                temp >> other;
                return (temp.currencyAmount < other.currencyAmount);
            }
            return (currencyAmount < other.currencyAmount);
        }

        bool operator>=(const Currency& other) const
        {
            if (this != &other)
            {
                Currency temp = *this;
                temp >> other;
                return (temp.currencyAmount >= other.currencyAmount);
            }
            return (currencyAmount >= other.currencyAmount);
        }

        bool operator<=(const Currency& other) const
        {
            if (this != &other)
            {
                Currency temp = *this;
                temp >> other;
                return (temp.currencyAmount <= other.currencyAmount);
            }
            return (currencyAmount <= other.currencyAmount);
        }

        Currency& operator>>(const Currency& other)
        {
            currencyName = other.currencyName;
            float baseCurrencyAmount = currencyAmount / currencyConversionRate;
            currencyAmount = baseCurrencyAmount * other.currencyConversionRate;
            currencyConversionRate = other.currencyConversionRate;

            return *this;
        }

        Currency operator+(const Currency& other) const
        {
            Currency currencySum = *this;

            if (*this != other)
            {
                currencySum >> other;
                currencySum.currencyAmount = currencySum.currencyAmount + other.currencyAmount;
            }
            else
            {
                currencySum.currencyAmount = currencyAmount + other.currencyAmount;
            }

            return currencySum;
        }

        Currency operator-(const Currency& other) const
        {
            Currency currencySub = *this;

            if (*this != other)
            {
                currencySub >> other;
                currencySub.currencyAmount = currencySub.currencyAmount - other.currencyAmount;
            }
            else
            {
                currencySub.currencyAmount = currencyAmount - other.currencyAmount;
            }

            return currencySub;
        }

        Currency operator*(int scalar) const
        {
            Currency currencyMult = *this;
            currencyMult.currencyAmount = currencyMult.currencyAmount * scalar;

            return currencyMult;

        }

        Currency operator/(int scalar) const
        {
            Currency currencyDiv = *this;
            currencyDiv.currencyAmount = currencyDiv.currencyAmount / scalar;

            return currencyDiv;
        }

        void operator++()
        {
            currencyAmount++;
        }

        void operator++(int)
        {
            Currency* temp = this;
            temp->currencyAmount++;
        }

        void operator--()
        {
            currencyAmount--;
        }

        void operator--(int)
        {
            Currency* temp = this;
            temp->currencyAmount--;
            temp = nullptr;
        }

        void operator+=(int scalar)
        {
            currencyAmount += scalar;
        }

        void operator-=(int scalar)
        {
            currencyAmount -= scalar;
        }

        void operator*=(int scalar)
        {
            currencyAmount *= scalar;
        }

        void operator/=(int scalar)
        {
            currencyAmount /= scalar;
        }

        friend ostream& operator<<(ostream& out, const Currency& currency);

        ~Currency() {}
};

ostream& operator<< (ostream& out, const Currency& currency)
{
    out << "Currency (" << currency.currencyName << ", " << currency.currencyAmount << ", " << currency.currencyConversionRate << " )" << endl;
    return out;
}

int main()
{
    Currency php("PHP", 100.00f, 58.38);
    Currency ksh("Ksh.", 100.00f, 128.94);
    Currency php1("PHP", 100.00f, 58.38);
    Currency ksh1("Ksh.", 100.00f, 128.94);
    Currency php2("PHP", 100.00f, 58.38);
    Currency ksh2("Ksh.", 100.00f, 128.94);

    // cout << (php >= ksh) << endl;

    // ksh++;
    // cout << (ksh) << endl;
    // ++ksh;
    // cout << (ksh) << endl;

    // ksh--;
    // cout << (ksh) << endl;
    // --ksh;
    // cout << (ksh) << endl;

    // cout << "ksh to php: " << (ksh1 >> php1) << endl;
    // cout << "php to ksh: " << (php2 >> ksh2) << endl;
    
    // cout << "ksh + php: " << (ksh + php) << endl;
    // cout << "php + ksh: " << (php + ksh) << endl;
    // cout << "php - ksh: " << (php - ksh) << endl;
    // cout << "ksh + ksh: " << (ksh + ksh) << endl;

    // cout << "ksh * 4: " << (ksh * 4) << endl;
    // cout << "ksh / 4: " << (ksh / 4) << endl;
    // cout << "php * 7: " << (php * 7) << endl;
    // cout << "php / 7: " << (php / 7) << endl;

    // php += 7;
    // php -= 7;
    // php *= 7;
    // php /= 7;
    // cout << "php ?(+ - * /)= 7: " << (php) << endl;

    // cout << "php == ksh: " << (php == ksh) << endl;
    // cout << "php != ksh: " <<  (php != ksh) << endl << endl;

    // cout << "ksh == ksh: " <<  (ksh == ksh) << endl;
    // cout << "ksh != ksh: " <<  (ksh != ksh) << endl;

    return 0;
}