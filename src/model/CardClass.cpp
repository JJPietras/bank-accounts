//
// Created by student on 21.12.2019.
//

#include <sstream>

#include "model/CardClass.h"
#include "model/Amount.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

CardClass::CardClass(AmountUPtr baseWithdrawCost, AmountUPtr payIntoCost) : baseWithdrawCost(move(baseWithdrawCost)), payIntoCost(move(payIntoCost)) {
    if (this->baseWithdrawCost == nullptr) {
        throw CardClassConstructionException(NULL_AMOUNT);
    }
    if (*this->baseWithdrawCost < Amount(0, 0, this->baseWithdrawCost->getCurrency())) {
        throw CardClassConstructionException(NEGATIVE_AMOUNT);
    }
    if (this->payIntoCost == nullptr) {
        throw CardClassConstructionException(NULL_AMOUNT);
    }
    if (*this->payIntoCost < Amount(0, 0, this->payIntoCost->getCurrency())) {
        throw CardClassConstructionException(NEGATIVE_AMOUNT);
    }
}

CardClass::~CardClass() = default;

const AmountUPtr &CardClass::getBaseWithdrawCost() const {
    return baseWithdrawCost;
}

const AmountUPtr &CardClass::getPayIntoCost() const {
    return payIntoCost;
}

string CardClass::toString() {
    stringstream stream;
    stream << briefCardClassInfo() << ", Base Withdraw Cost: " << *getBaseWithdrawCost() << ", Pay Into Cost: " << *getPayIntoCost();
    return stream.str();
}
