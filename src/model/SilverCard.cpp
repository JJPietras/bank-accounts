//
// Created by student on 21.12.2019.
//

#include "model/SilverCard.h"
#include "model/Amount.h"
#include "model/Account.h"
#include "model/Client.h"
#include "model/ClientType.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

SilverCard::SilverCard(AmountUPtr baseWithdrawCost, AmountUPtr payIntoCost) : CardClass(move(baseWithdrawCost), move(payIntoCost)) {}

SilverCard::~SilverCard() = default;

AmountSPtr SilverCard::calculateWithdrawCost(const AccountSPtr &account) const {
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

    return make_shared<Amount>(*getBaseWithdrawCost() * avgCoefficient * 0.8f);
}

bool SilverCard::payIntoAccount(AmountUPtr amount, const AccountSPtr &account) {
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

string SilverCard::briefCardClassInfo() {
    return "Silver Card";
}
