//
// Created by student on 21.12.2019.
//

#include <cmath>

#include "model/Amount.h"
#include "exceptions/AmountExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

Amount::Amount(long mainUnit, short fractionalUnit, CurrencyType currency) : mainUnit(mainUnit), fractionalUnit(fractionalUnit), currency(currency) {
    if (mainUnit * fractionalUnit < 0) {
        throw AmountConstructionException(SIGNS);
    }
    if (abs(fractionalUnit) > 99) {
        throw AmountConstructionException(FRACTIONAL + to_string(fractionalUnit) + ".");
    }
}

Amount::~Amount() = default;

long Amount::getMainUnit() const {
    return mainUnit;
}

short Amount::getFractionalUnit() const {
    return fractionalUnit;
}

CurrencyType Amount::getCurrency() const {
    return currency;
}

bool Amount::operator==(const Amount &amount) const {
    if (this->currency != amount.currency) {
        throw AmountOperatorException(CURRENCY);
    }
    return (mainUnit == amount.mainUnit && fractionalUnit == amount.fractionalUnit);
}

Amount Amount::operator+(const Amount &amount) const {
    if (this->currency != amount.currency) {
        throw AmountOperatorException(CURRENCY);
    }

    long newMainUnit = mainUnit + amount.mainUnit;
    auto newFractionalUnit = (short) (fractionalUnit + amount.fractionalUnit);
    long helper = newMainUnit * newFractionalUnit;
    if (helper < 0) {
        if (newMainUnit > 0) {
            newMainUnit--;
            newFractionalUnit += 100;
        } else {
            newMainUnit++;
            newFractionalUnit -= 100;
        }
    }
    if (newFractionalUnit > 99) {
        newMainUnit++;
        newFractionalUnit -= 100;
    } else if (newFractionalUnit < -99) {
        newMainUnit--;
        newFractionalUnit += 100;
    }
    return Amount(newMainUnit, newFractionalUnit, currency);
}

Amount Amount::operator-(const Amount &amount) const {
    return *this + Amount(-amount.mainUnit, (short) -amount.fractionalUnit, amount.currency);
}

Amount Amount::operator*(const double &value) const {
    long mainTemp = mainUnit * (long) ceil(((int) (value * 10000L)) / 10.0);
    int fractTemp = ceil(fractionalUnit * (value * 10));
    auto fract = (short) ceil(((int) (mainTemp % 1000L) + fractTemp) / 10.0);
    long mainResult = mainTemp / 1000L;
    if (abs(fract) > 99) {
        mainResult += fract / 100;
        fract %= 100;
    }
    return Amount(mainResult, fract, currency);
}

bool Amount::operator>(const Amount &amount) const {
    if (this->currency != amount.currency) {
        throw AmountOperatorException(CURRENCY);
    }
    return (mainUnit > amount.mainUnit || (mainUnit == amount.mainUnit && fractionalUnit > amount.fractionalUnit));
}

bool Amount::operator<(const Amount &amount) const {
    return amount > *this;
}

ostream &operator<<(ostream &out, const Amount &amount) {
    out << amount.toString();
    return out;
}

string Amount::toString(bool symbol) const {
    string out = to_string(mainUnit) + ".";
    if (fractionalUnit != 0 && abs(fractionalUnit) < 10) {
        out.append("0");
    }
    out.append(to_string(abs(fractionalUnit)));
    if (mainUnit == 0 && fractionalUnit < 0) {
        out.insert(out.begin(), '-');
    }
    if (fractionalUnit == 0) {
        out.append("0");
    }
    out.append(currencyToString(currency, symbol));
    return out;
}

string Amount::currencyToString(CurrencyType currency, bool symbol) {
    string currencyStr;
    switch (currency) {
        case USD:
            currencyStr = symbol ? "$" : "USD";
            break;
        case EUR:
            currencyStr = symbol ? "€" : "EUR";
            break;
        case GBP:
            currencyStr = symbol ? "£" : "GBP";
            break;
        case PLN:
            currencyStr = symbol ? "zł" : "PLN";
            break;
    }
    return currencyStr;
}
