//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_AMOUNT_H
#define BANKKONTA_AMOUNT_H


#include <memory>

#include "../enum/CurrencyType.h"

class Amount {
private:
    long mainUnit;
    short fractionalUnit;
    CurrencyType currency;

public:
    Amount(long mainUnit, short fractionalUnit, CurrencyType currency);

    ~Amount();

    long getMainUnit() const;

    short getFractionalUnit() const;

    CurrencyType getCurrency() const;

    bool operator==(const Amount &amount) const;

    Amount operator+(const Amount &amount) const;

    Amount operator-(const Amount &amount) const;

    Amount operator*(const double &value) const;

    bool operator>(const Amount &amount) const;

    bool operator<(const Amount &amount) const;

    friend std::ostream &operator<<(std::ostream &out, const Amount &amount);

    std::string toString(bool symbol = true) const;

    static std::string currencyToString(CurrencyType currency, bool symbol);
};

typedef std::shared_ptr<Amount> AmountSPtr;
typedef std::unique_ptr<Amount> AmountUPtr;

#endif //BANKKONTA_AMOUNT_H
