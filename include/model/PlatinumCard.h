//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_PLATINUMCARD_H
#define BANKKONTA_PLATINUMCARD_H


#include "CardClass.h"

class PlatinumCard : public CardClass {
public:
    PlatinumCard(std::unique_ptr<Amount> baseWithdrawCost, std::unique_ptr<Amount> payIntoCost);

    ~PlatinumCard() override;

    std::shared_ptr<Amount> calculateWithdrawCost(const std::shared_ptr<Account> &account) const override;

    bool payIntoAccount(std::unique_ptr<Amount> amount, const std::shared_ptr<Account> &account) override;

    std::string briefCardClassInfo() override;

};

typedef std::shared_ptr<PlatinumCard> PlatinumCardSPtr;
typedef std::unique_ptr<PlatinumCard> PlatinumCardUPtr;

#endif //BANKKONTA_PLATINUMCARD_H
