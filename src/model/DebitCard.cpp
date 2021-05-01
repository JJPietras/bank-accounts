//
// Created by student on 21.12.2019.
//

#include <sstream>
#include <boost/uuid/uuid_io.hpp>

#include "model/DebitCard.h"
#include "model/CardClass.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Account.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

DebitCard::DebitCard(const ClientSPtr &owner, const AccountSPtr &account) : Card(owner, account) {}

DebitCard::~DebitCard() = default;

bool DebitCard::payIntoAccount(AmountUPtr amount) {
    if (amount == nullptr) {
        throw CardMethodException(NULL_AMOUNT);
    }
    if (*amount < Amount(0L, 0, amount->getCurrency())) {
        throw CardMethodException(NEGATIVE_AMOUNT);
    }
    return getCardClass()->payIntoAccount(AmountUPtr(new Amount(*amount)), getAccount());
}

string DebitCard::briefCardInfo() const {
    string owner = ", Owner: " + getOwner()->getFirstName() + " " + getOwner()->getLastName();
    return "Debit Card, Class: " + getCardClass()->briefCardClassInfo() + owner;
}

string DebitCard::toString() const {
    stringstream stream;

    stream << "=========================================================\n";
    stream << "Type:              | Credit Card" << "\n";
    stream << "Class:             | " << getCardClass()->briefCardClassInfo() << "\n";
    stream << "Currency           | " << Amount::currencyToString(getAmount()->getCurrency(), false) << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "Owner:\n" << getOwner()->briefClientInfo() << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "UUID:              | " << boost::uuids::to_string(*getUuid()) << "\n";
    stream << "=========================================================";

    return stream.str();
}