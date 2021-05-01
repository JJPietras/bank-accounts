//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_SILVERCARD_H
#define BANKKONTA_SILVERCARD_H


#include "CardClass.h"

class SilverCard : public CardClass {
public:
    SilverCard(std::unique_ptr<Amount> baseWithdrawCost, std::unique_ptr<Amount> payIntoCost);

    ~SilverCard() override;

    std::shared_ptr<Amount> calculateWithdrawCost(const std::shared_ptr<Account> &account) const override;

    bool payIntoAccount(std::unique_ptr<Amount> amount, const std::shared_ptr<Account> &account) override;

    std::string briefCardClassInfo() override;

};

typedef std::shared_ptr<SilverCard> SilverCardSPtr;
typedef std::unique_ptr<SilverCard> SilverCardUPtr;

#endif //BANKKONTA_SILVERCARD_H
