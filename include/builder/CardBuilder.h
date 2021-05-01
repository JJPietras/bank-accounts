//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CARDBUILDER_H
#define BANKKONTA_CARDBUILDER_H


#include <memory>
#include <boost/uuid/uuid.hpp>

#include "enum/CardEnum.h"

class CardClass;

class Amount;

class Client;

class Account;

class Card;

class CardBuilder {
private:
    std::shared_ptr<CardClass> cardClass = nullptr;
    std::shared_ptr<Amount> balance = nullptr;
    std::shared_ptr<Client> owner = nullptr;
    std::shared_ptr<Account> account = nullptr;
    std::shared_ptr<CardEnum> cardType = nullptr;

public:
    CardBuilder();

    ~CardBuilder();

    std::shared_ptr<CardBuilder> setCardClass(const std::shared_ptr<CardClass> &_cardClass);

    std::shared_ptr<CardBuilder> setBalance(const std::shared_ptr<Amount> &_balance);

    std::shared_ptr<CardBuilder> setOwner(const std::shared_ptr<Client> &_owner);

    std::shared_ptr<CardBuilder> setAccount(const std::shared_ptr<Account> &_account);

    std::shared_ptr<CardBuilder> setCardType(const std::shared_ptr<CardEnum> &_cardType);

    std::shared_ptr<Card> build();

};

typedef std::shared_ptr<CardBuilder> CardBuilderSPtr;
typedef std::unique_ptr<CardBuilder> CardBuilderUPtr;

#endif //BANKKONTA_CARDBUILDER_H
