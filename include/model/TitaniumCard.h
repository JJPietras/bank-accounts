//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_TITANIUMCARD_H
#define BANKKONTA_TITANIUMCARD_H


#include "CardClass.h"

class TitaniumCard : public CardClass {
public:
    TitaniumCard(std::unique_ptr<Amount> baseWithdrawCost, std::unique_ptr<Amount> payIntoCost);

    ~TitaniumCard() override;

    std::shared_ptr<Amount> calculateWithdrawCost(const std::shared_ptr<Account> &account) const override;

    bool payIntoAccount(std::unique_ptr<Amount> amount, const std::shared_ptr<Account> &account) override;

    std::string briefCardClassInfo() override;

};

typedef std::shared_ptr<TitaniumCard> TitaniumCardSPtr;
typedef std::unique_ptr<TitaniumCard> TitaniumCardUPtr;

#endif //BANKKONTA_TITANIUMCARD_H
