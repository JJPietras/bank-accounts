//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CARDCLASS_H
#define BANKKONTA_CARDCLASS_H


#include <memory>

#include "enum/CurrencyType.h"

class Amount;

class Account;

class CardClass {
private:
    std::unique_ptr<Amount> baseWithdrawCost;
    std::unique_ptr<Amount> payIntoCost;

public:
    CardClass(std::unique_ptr<Amount> baseWithdrawCost, std::unique_ptr<Amount> payIntoCost);

    virtual ~CardClass();

    const std::unique_ptr<Amount> &getBaseWithdrawCost() const;

    const std::unique_ptr<Amount> &getPayIntoCost() const;

    virtual std::shared_ptr<Amount> calculateWithdrawCost(const std::shared_ptr<Account> &account) const = 0;

    virtual bool payIntoAccount(std::unique_ptr<Amount> amount, const std::shared_ptr<Account> &account) = 0;

    virtual std::string briefCardClassInfo() = 0;

    std::string toString();

};

typedef std::shared_ptr<CardClass> CardClassSPtr;
typedef std::unique_ptr<CardClass> CardClassUPtr;

#endif //BANKKONTA_CARDCLASS_H
