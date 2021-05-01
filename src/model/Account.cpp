//
// Created by student on 21.12.2019.
//

#include <boost/date_time/local_time/posix_time_zone.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "model/Account.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Card.h"
#include "exceptions/AccountExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

Account::Account(AmountUPtr amount) : amount(move(amount)) {
    if (this->amount == nullptr) {
        throw AccountConstructionException(NULL_AMOUNT);
    }
    boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
    boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
    openingTime.reset(new boost::local_time::local_date_time(time, zone));
    uuid.reset(new boost::uuids::uuid(boost::uuids::random_generator()()));
}

Account::Account(unique_ptr<boost::local_time::local_date_time> openingTime, AmountUPtr amount) : openingTime(move(openingTime)), amount(move(amount)) {
    if (this->amount == nullptr) {
        throw AccountConstructionException(NULL_AMOUNT);
    }
    if (this->openingTime == nullptr) {
        throw AccountConstructionException(NULL_DATE_TIME);
    }
    uuid.reset(new boost::uuids::uuid(boost::uuids::random_generator()()));
}

Account::~Account() = default;

const unique_ptr<boost::uuids::uuid> &Account::getUuid() const {
    return uuid;
}

const AmountUPtr &Account::getAmount() const {
    return amount;
}

void Account::setAmount(AmountUPtr _amount) {
    if (_amount == nullptr) {
        throw AccountModificationException(NULL_AMOUNT);
    }
    if (_amount->getCurrency() != amount->getCurrency()) {
        throw AccountModificationException(CURRENCY);
    }
    Account::amount = move(_amount);
}

const unique_ptr<boost::local_time::local_date_time> &Account::getOpeningTime() const {
    return openingTime;
}

const unique_ptr<boost::local_time::local_date_time> &Account::getClosureTime() const {
    return closureTime;
}

CurrencyType Account::getCurrency() const {
    return amount->getCurrency();
}

const vector<ClientSPtr> &Account::getOwners() const {
    return owners;
}

bool Account::addOwner(const ClientSPtr &owner) {
    if (find(owners.begin(), owners.end(), owner) != owners.end()) {
        return false;
    }
    owners.emplace_back(owner);
    return true;
}

bool Account::removeOwner(const ClientSPtr &owner) {
    if (owner == nullptr) {
        throw AccountMethodException(NULL_CLIENT);
    }
    auto it = find(owners.begin(), owners.end(), owner);
    if (it != owners.end()) {
        owners.erase(it);
        return true;
    }
    return false;
}

const vector<CardSPtr> &Account::getCards() const {
    return cards;
}

bool Account::addCard(const CardSPtr &card) {
    if (card == nullptr) {
        throw AccountMethodException(NULL_CARD);
    }
    if (find(cards.begin(), cards.end(), card) != cards.end()) {
        return false;
    }
    cards.emplace_back(card);
    return true;
}

bool Account::removeCard(const CardSPtr &card) {
    if (card == nullptr) {
        throw AccountMethodException(NULL_CARD);
    }
    auto it = find(cards.begin(), cards.end(), card);
    if (it != cards.end()) {
        cards.erase(it);
        return true;
    }
    return false;
}

AmountUPtr Account::closeAccount() {
    if (closureTime != nullptr || owners.empty()) {
        throw AccountMethodException(NO_OWNER_CLOSED);
    }
    closureTime.reset(new boost::local_time::local_date_time(boost::posix_time::second_clock::local_time(), openingTime->zone()));
    AmountUPtr money(new Amount(*getAmount()));
    setAmount(AmountUPtr(new Amount(0, 0, getAmount()->getCurrency())));
    return money;
}

string Account::toString() const {
    stringstream stream;

    stream << "=========================================================\n";
    stream << "Type:            | Joint Account" << "\n";
    stream << "Currency         | " << Amount::currencyToString(getAmount()->getCurrency(), false) << "\n";
    stream << "Amount:          | " << *getAmount() << "\n";
    stream << "Transfer Cost:   | " << *calculateTransferCost() << "%\n";
    stream << "Opening Date:    | " << getOpeningTime()->to_string() << "\n";
    stream << "Closure Date:    | " << (getClosureTime() == nullptr ? "Account is active" : getClosureTime()->to_string()) << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "Owner:";
    stream << "---------------------------------------------------------\n";
    stream << "Cards:           | " << getCards().size() << "\n";

    int i = 0;
    for (const auto &c : cards) {
        stream << "    " << i << '.' << c->briefCardInfo() << "\n";
        i++;
    }

    stream << "---------------------------------------------------------\n";
    stream << "UUID:            | " << boost::uuids::to_string(*getUuid()) << "\n";
    stream << "=========================================================";

    return stream.str();
}
