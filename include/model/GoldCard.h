//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_GOLDCARD_H
#define BANKKONTA_GOLDCARD_H


#include "CardClass.h"

class GoldCard : public CardClass {
public:
    GoldCard(std::unique_ptr<Amount> baseWithdrawCost, std::unique_ptr<Amount> payIntoCost);

    ~GoldCard() override;

    std::shared_ptr<Amount> calculateWithdrawCost(const std::shared_ptr<Account> &account) const override;

    bool payIntoAccount(std::unique_ptr<Amount> amount, const std::shared_ptr<Account> &account) override;

    std::string briefCardClassInfo() override;

};

typedef std::shared_ptr<GoldCard> GoldCardSPtr;
typedef std::unique_ptr<GoldCard> GoldCardUPtr;

#endif //BANKKONTA_GOLDCARD_H
