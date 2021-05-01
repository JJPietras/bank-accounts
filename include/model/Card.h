//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CARD_H
#define BANKKONTA_CARD_H


#include <memory>

#include "boost/uuid/uuid.hpp"

class Client;

class CardClass;

class Amount;

class Account;

class Card {
private:
    std::unique_ptr<boost::uuids::uuid> uuid;
    std::shared_ptr<Client> owner;
    std::shared_ptr<Account> account;
    std::shared_ptr<CardClass> cardClass;

public:
    Card(std::shared_ptr<Client> owner, std::shared_ptr<Account> account);

    virtual ~Card();

    const std::unique_ptr<boost::uuids::uuid> &getUuid() const;

    const std::shared_ptr<Client> &getOwner() const;

    const std::shared_ptr<Account> &getAccount() const;

    const std::shared_ptr<CardClass> &getCardClass() const;

    void setCardClass(const std::shared_ptr<CardClass> &_cardClass);

    const std::unique_ptr<Amount> &getAmount() const;

    std::shared_ptr<Amount> calculateWithdrawCost() const;

    virtual bool makePayment(std::unique_ptr<Amount> amount);

    virtual std::string briefCardInfo() const = 0;

    virtual std::string toString() const = 0;

};

typedef std::shared_ptr<Card> CardSPtr;
typedef std::unique_ptr<Card> CardUPtr;

#endif //BANKKONTA_CARD_H
