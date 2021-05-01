//
// Created by student on 21.12.2019.
//

#include <utility>
#include <memory>
#include <boost/uuid/uuid_generators.hpp>

#include "model/Card.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/Account.h"
#include "model/CardClass.h"
#include "model/StandardCard.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

Card::Card(ClientSPtr owner, AccountSPtr account) : owner(move(owner)), account(move(account)) {
    if (this->owner == nullptr) {
        throw CardConstructionException(NULL_OWNER);
    }
    if (this->account == nullptr) {
        throw CardConstructionException(NULL_ACCOUNT);
    }
    this->uuid.reset(new boost::uuids::uuid(boost::uuids::random_generator()()));
    auto baseWithdrawCost = AmountUPtr(new Amount(2L, 0, getAmount()->getCurrency()));
    auto payIntoCost = AmountUPtr(new Amount(1L, 0, getAmount()->getCurrency()));
    this->cardClass.reset<CardClass>(new StandardCard(move(baseWithdrawCost), move(payIntoCost)));
}

Card::~Card() = default;

const unique_ptr<boost::uuids::uuid> &Card::getUuid() const {
    return uuid;
}

const ClientSPtr &Card::getOwner() const {
    return owner;
}

const AccountSPtr &Card::getAccount() const {
    return account;
}

const CardClassSPtr &Card::getCardClass() const {
    return cardClass;
}

void Card::setCardClass(const CardClassSPtr &_cardClass) {
    if (_cardClass == nullptr) {
        throw CardModificationException(NULL_CARD_CLASS);
    }
    Card::cardClass = _cardClass;
}

const AmountUPtr &Card::getAmount() const {
    return account->getAmount();
}

AmountSPtr Card::calculateWithdrawCost() const {
    return cardClass->calculateWithdrawCost(account);
}

bool Card::makePayment(AmountUPtr amount) {
    if (amount == nullptr) {
        throw CardMethodException(NULL_AMOUNT);
    }
    if (amount->getCurrency() != getAmount()->getCurrency()) {
        throw CardMethodException(CURRENCY);
    }
    if (*amount < Amount(0L, 0, amount->getCurrency())) {
        throw CardMethodException(NEGATIVE_AMOUNT);
    }
    if (*amount > *account->getAmount()) {
        return false;
    }
    account->setAmount(AmountUPtr(new Amount(*account->getAmount() - *amount)));
    return true;
}
