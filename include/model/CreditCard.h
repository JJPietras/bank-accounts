//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CREDITCARD_H
#define BANKKONTA_CREDITCARD_H


#include "Card.h"
#include "enum/CurrencyType.h"

class Amount;

class CreditCard : public Card {
private:
    std::unique_ptr<Amount> balance;

public:
    CreditCard(const std::shared_ptr<Client> &owner, const std::shared_ptr<Account> &account);

    ~CreditCard() override;

    const std::unique_ptr<Amount> &getBalance() const;

    bool payBack(const std::unique_ptr<Amount> &amount);

    bool makePayment(std::unique_ptr<Amount> amount) override;

    std::string briefCardInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<CreditCard> CreditCardSPtr;
typedef std::unique_ptr<CreditCard> CreditCardUPtr;

#endif //BANKKONTA_CREDITCARD_H
