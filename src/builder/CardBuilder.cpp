//
// Created by student on 21.12.2019.
//

#include "builder/CardBuilder.h"
#include "model/CurrencyAccount.h"
#include "model/CardClass.h"
#include "model/DebitCard.h"
#include "model/CreditCard.h"
#include "model/Account.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Card.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

CardBuilder::CardBuilder() = default;

CardBuilder::~CardBuilder() = default;

CardBuilderSPtr CardBuilder::setCardClass(const CardClassSPtr &_cardClass) {
    if (_cardClass == nullptr) {
        throw CardBuilderModificationException(NULL_CARD_CLASS);
    }
    cardClass = _cardClass;
    return make_shared<CardBuilder>(*this);
}

CardBuilderSPtr CardBuilder::setBalance(const AmountSPtr &_balance) {
    if (_balance == nullptr) {
        throw CardBuilderModificationException(NULL_AMOUNT);
    }
    if (*_balance < Amount(0L, 0, _balance->getCurrency())) {
        throw CardBuilderModificationException(NEGATIVE_AMOUNT);
    }
    balance = _balance;
    return make_shared<CardBuilder>(*this);
}

CardBuilderSPtr CardBuilder::setOwner(const ClientSPtr &_owner) {
    if (_owner == nullptr) {
        throw CardBuilderModificationException(NULL_CLIENT);
    }
    owner = _owner;
    return make_shared<CardBuilder>(*this);
}

CardBuilderSPtr CardBuilder::setAccount(const AccountSPtr &_account) {
    if (_account == nullptr) {
        throw CardBuilderModificationException(NULL_CLIENT);
    }
    account = _account;
    return make_shared<CardBuilder>(*this);
}

CardBuilderSPtr CardBuilder::setCardType(const shared_ptr<CardEnum> &_cardType) {
    if (_cardType == nullptr) {
        throw CardBuilderModificationException(NULL_CARD_TYPE);
    }
    cardType = _cardType;
    return make_shared<CardBuilder>(*this);
}

CardSPtr CardBuilder::build() {
    if (owner == nullptr || account == nullptr || cardType == nullptr) {
        throw CardBuilderBuildException(CARD_CRUCIAL_MISSING);
    }

    CardSPtr card;

    switch (*cardType) {
        case DebitCardType:
            card.reset(new DebitCard(owner, account));
            break;
        case CreditCardType:
            if (dynamic_pointer_cast<CurrencyAccount>(account) == nullptr) {
                throw CardBuilderBuildException(WRONG_ACCOUNT);
            }
            card.reset(new CreditCard(owner, account));
            if (balance != nullptr) {
                if (balance->getCurrency() != account->getCurrency()) {
                    throw CardBuilderBuildException(WRONG_CURRENCY);
                }
                card->makePayment(AmountUPtr(new Amount(*balance)));
                account->setAmount(AmountUPtr(new Amount(*account->getAmount() + *balance)));
                balance = nullptr;
            }
            break;
    }

    if (cardClass != nullptr) {
        card->setCardClass(cardClass);
        cardClass = nullptr;
    }

    owner = nullptr;
    account = nullptr;

    return card;
}