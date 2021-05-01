//
// Created by student on 21.12.2019.
//

#include <boost/uuid/uuid_io.hpp>

#include "model/CreditCard.h"
#include "model/CardClass.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/Account.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

CreditCard::CreditCard(const ClientSPtr &owner, const AccountSPtr &account) : Card(owner, account) {
    balance.reset(new Amount(0, 0, getAmount()->getCurrency()));
}

CreditCard::~CreditCard() = default;

const AmountUPtr &CreditCard::getBalance() const {
    return balance;
}

bool CreditCard::payBack(const AmountUPtr &amount) {
    if (amount == nullptr) {
        throw CardMethodException(NULL_AMOUNT);
    }
    if (*amount < Amount(0L, 0, amount->getCurrency())) {
        throw CardMethodException(NEGATIVE_AMOUNT);
    }
    if (amount->getCurrency() != getAmount()->getCurrency()) {
        throw CardMethodException(CURRENCY);
    }

    auto account = getAccount();
    auto owner = getOwner();

    if (*balance - *amount < Amount(0L, 0, amount->getCurrency()) || *balance == *amount) {
        if (*balance - *amount < Amount(0L, 0, amount->getCurrency())) {
            owner->setFounds(AmountUPtr(new Amount(*owner->getFounds()[amount->getCurrency()] + (*amount - *balance))));
        }
        account->setAmount(AmountUPtr(new Amount(*getAmount() + *balance)));
        balance.reset(new Amount(0L, 0, amount->getCurrency()));
        return true;
    }
    account->setAmount(AmountUPtr(new Amount(*getAmount() + *amount)));
    balance.reset(new Amount(*balance - *amount));
    return false;
}

bool CreditCard::makePayment(AmountUPtr amount) {
    if (amount == nullptr) {
        throw CardMethodException(NULL_AMOUNT);
    }
    if (amount->getCurrency() != getAmount()->getCurrency()) {
        throw CardMethodException(MULTIPLE_CURRENCIES);
    }
    balance.reset(new Amount(*balance + *amount));
    return Card::makePayment(move(amount));
}

string CreditCard::briefCardInfo() const {
    string owner = ", Owner: " + getOwner()->getFirstName() + " " + getOwner()->getLastName();
    return "Credit Card, Class: " + getCardClass()->briefCardClassInfo() + " Balance: " + balance->toString() + ", Max Amount: " + (*getAccount()->getAmount() + *balance).toString() + owner;
}

string CreditCard::toString() const {
    stringstream stream;

    stream << "=========================================================\n";
    stream << "Type:             | Credit Card" << "\n";
    stream << "Currency          | " << Amount::currencyToString(getAmount()->getCurrency(), false) << "\n";
    stream << "Max Amount:       | " << *getAmount() + *balance << "\n";
    stream << "Remaining Amount: | " << *getAmount() << "\n";
    stream << "Balance:          | " << *balance << "%\n";
    stream << "---------------------------------------------------------\n";
    stream << "Owner:\n" << getOwner()->briefClientInfo() << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "UUID:             | " << boost::uuids::to_string(*getUuid()) << "\n";
    stream << "=========================================================";

    return stream.str();
}
