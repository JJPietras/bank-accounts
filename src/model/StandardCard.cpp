//
// Created by student on 21.12.2019.
//

#include <memory>
#include <vector>
#include "model/StandardCard.h"

#include "model/Amount.h"
#include "model/Account.h"
#include "model/Client.h"
#include "model/ClientType.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

StandardCard::StandardCard(AmountUPtr baseWithdrawCost, AmountUPtr payIntoCost) : CardClass(move(baseWithdrawCost), move(payIntoCost)) {}

StandardCard::~StandardCard() = default;

AmountSPtr StandardCard::calculateWithdrawCost(const AccountSPtr &account) const {
    if (account == nullptr) {
        throw CardClassMethodException(NULL_ACCOUNT);
    }
    if (account->getOwners().empty()) {
        throw CardClassMethodException(NO_OWNER_CLOSED);
    }

    float avgCoefficient = 0.0f;

    for (const auto &o : account->getOwners()) {
        avgCoefficient += o->getClientType()->calculateServiceCostCoefficient(o->getCreditworthiness(), make_shared<Amount>(*getBaseWithdrawCost()));
    }

    avgCoefficient /= account->getOwners().size();

    return make_shared<Amount>(*getBaseWithdrawCost() * avgCoefficient);
}

bool StandardCard::payIntoAccount(AmountUPtr amount, const AccountSPtr &account) {
    if (amount == nullptr) {
        throw CardClassMethodException(NULL_AMOUNT);
    }
    if (*amount < Amount(0, 0, amount->getCurrency())) {
        throw CardClassMethodException(NEGATIVE_AMOUNT);
    }
    if (account == nullptr) {
        throw CardClassMethodException(NULL_ACCOUNT);
    }

    if (amount->getCurrency() != account->getAmount()->getCurrency()) {
        return false;
    }

    account->setAmount(AmountUPtr(new Amount(*account->getAmount() + *amount - *getPayIntoCost())));

    return true;
}

string StandardCard::briefCardClassInfo() {
    return "Standard Card";
}