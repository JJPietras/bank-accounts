//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_DEBITCARD_H
#define BANKKONTA_DEBITCARD_H


#include "Card.h"
#include "enum/CurrencyType.h"

class DebitCard : public Card {
public:
    DebitCard(const std::shared_ptr<Client> &owner, const std::shared_ptr<Account> &account);

    ~DebitCard() override;

    bool payIntoAccount(std::unique_ptr<Amount> amount);

    std::string briefCardInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<DebitCard> DebitCardSPtr;
typedef std::unique_ptr<DebitCard> DebitCardUPtr;

#endif //BANKKONTA_DEBITCARD_H
