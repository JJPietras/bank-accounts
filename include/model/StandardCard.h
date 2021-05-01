//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_STANDARDCARD_H
#define BANKKONTA_STANDARDCARD_H


#include "CardClass.h"

class StandardCard : public CardClass {
public:
    StandardCard(std::unique_ptr<Amount> baseWithdrawCost, std::unique_ptr<Amount> payIntoCost);

    ~StandardCard() override;

    std::shared_ptr<Amount> calculateWithdrawCost(const std::shared_ptr<Account> &account) const override;

    bool payIntoAccount(std::unique_ptr<Amount> amount, const std::shared_ptr<Account> &account) override;

    std::string briefCardClassInfo() override;

};

typedef std::shared_ptr<StandardCard> StandardCardSPtr;
typedef std::unique_ptr<StandardCard> StandardCardUPtr;

#endif //BANKKONTA_STANDARDCARD_H
