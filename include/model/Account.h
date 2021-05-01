//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_ACCOUNT_H
#define BANKKONTA_ACCOUNT_H

#include <memory>
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/date_time/local_time/local_date_time.hpp>

#include "enum/CurrencyType.h"

class Client;

class Card;

class Amount;

class Account {
private:
    std::unique_ptr<boost::uuids::uuid> uuid;
    std::unique_ptr<boost::local_time::local_date_time> openingTime;
    std::unique_ptr<boost::local_time::local_date_time> closureTime = nullptr;
    std::unique_ptr<Amount> amount;
    std::vector<std::shared_ptr<Client>> owners;
    std::vector<std::shared_ptr<Card>> cards;

public:
    explicit Account(std::unique_ptr<Amount> amount);

    Account(std::unique_ptr<boost::local_time::local_date_time> openingTime, std::unique_ptr<Amount> amount);

    virtual ~Account();

    const std::unique_ptr<boost::uuids::uuid> &getUuid() const;

    const std::unique_ptr<Amount> &getAmount() const;

    void setAmount(std::unique_ptr<Amount> _amount);

    const std::unique_ptr<boost::local_time::local_date_time> &getOpeningTime() const;

    const std::unique_ptr<boost::local_time::local_date_time> &getClosureTime() const;

    CurrencyType getCurrency() const;

    const std::vector<std::shared_ptr<Client>> &getOwners() const;

    virtual bool addOwner(const std::shared_ptr<Client> &owner);

    bool removeOwner(const std::shared_ptr<Client> &owner);

    const std::vector<std::shared_ptr<Card>> &getCards() const;

    bool addCard(const std::shared_ptr<Card> &card);

    bool removeCard(const std::shared_ptr<Card> &card);

    std::unique_ptr<Amount> closeAccount();

    virtual std::shared_ptr<Amount> calculateTransferCost() const = 0;

    virtual std::string briefAccountInfo() const = 0;

    virtual std::string toString() const;

};

typedef std::shared_ptr<Account> AccountSPtr;
typedef std::unique_ptr<Account> AccountUPtr;

#endif //BANKKONTA_ACCOUNT_H
