//
// Created by student on 21.12.2019.
//

#define CURRENCIES 4

#include <utility>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>

#include "model/Client.h"
#include "model/Card.h"
#include "model/Address.h"
#include "model/Account.h"
#include "model/Amount.h"
#include "model/Loan.h"
#include "model/ClientType.h"
#include "model/NewClient.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

Client::Client(string firstName, string lastName, string personalID, AddressUPtr registeredAddress)
        : firstName(move(firstName)), lastName(move(lastName)), personalID(move(personalID)), registeredAddress(move(registeredAddress)) {
    if (this->registeredAddress == nullptr) {
        throw ClientConstructionException(NULL_ADDRESS);
    }

    boost::trim(this->firstName);
    boost::trim(this->lastName);
    boost::trim(this->personalID);

    if (this->firstName.empty()) {
        throw ClientConstructionException(EMPTY_FIRST_NAME);
    }
    if (this->lastName.empty()) {
        throw ClientConstructionException(EMPTY_LAST_NAME);
    }
    if (this->personalID.empty()) {
        throw ClientConstructionException(EMPTY_PERSONAL_ID);
    }

    address.reset(new Address(*(this->registeredAddress)));

    CurrencyType type;
    for (int i = 0; i < CURRENCIES; i++) {
        type = static_cast<CurrencyType>(i);
        income.emplace_back(AmountUPtr(new Amount(0L, 0, type)));
        founds.emplace_back(AmountUPtr(new Amount(0L, 0, type)));
    }

    creditworthiness = 0;
    ownerOfCompany = false;

    vector<AmountUPtr> maxAmounts;
    maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
    maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
    maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
    maxAmounts.emplace_back(AmountUPtr(new Amount(2000L, 0, PLN)));

    clientType = ClientTypeSPtr(new NewClient(1.0f, 1, move(maxAmounts)));
}

Client::~Client() = default;

const string &Client::getFirstName() const {
    return firstName;
}

void Client::setFirstName(const string &_firstName) {
    string newFirstName = _firstName;
    boost::trim(newFirstName);
    if (newFirstName.empty()) {
        throw ClientModificationException(EMPTY_FIRST_NAME);
    }
    Client::firstName = newFirstName;
}

const string &Client::getLastName() const {
    return lastName;
}

void Client::setLastName(const string &_lastName) {
    string newLastName = _lastName;
    boost::trim(newLastName);
    if (newLastName.empty()) {
        throw ClientModificationException(EMPTY_LAST_NAME);
    }
    Client::lastName = newLastName;
}

const string &Client::getPersonalID() const {
    return personalID;
}

short Client::getCreditworthiness() const {
    return creditworthiness;
}

void Client::setCreditworthiness(short _creditworthiness) {
    Client::creditworthiness = _creditworthiness;
}

bool Client::isOwnerOfCompany() const {
    return ownerOfCompany;
}

void Client::setOwnerOfCompany(bool _ownerOfCompany) {
    Client::ownerOfCompany = _ownerOfCompany;
}

const AddressUPtr &Client::getRegisteredAddress() const {
    return registeredAddress;
}

void Client::setRegisteredAddress(AddressUPtr _registeredAddress) {
    if (_registeredAddress == nullptr) {
        throw ClientModificationException(NULL_ADDRESS);
    }
    Client::registeredAddress = move(_registeredAddress);
}

const AddressUPtr &Client::getAddress() const {
    return address;
}

void Client::setAddress(AddressUPtr _address) {
    if (_address == nullptr) {
        throw ClientModificationException(NULL_ADDRESS);
    }
    Client::address = move(_address);
}

const vector<AmountUPtr> &Client::getIncome() const {
    return income;
}

void Client::setIncome(AmountUPtr _income) {
    if (_income == nullptr) {
        throw ClientModificationException(NULL_AMOUNT);
    }
    if (_income->getMainUnit() < 0) {
        throw ClientModificationException(NEGATIVE_INCOME);
    }
    income[_income->getCurrency()] = move(_income);
}

const vector<AmountUPtr> &Client::getFounds() const {
    return founds;
}

void Client::setFounds(AmountUPtr _founds) {
    if (_founds == nullptr) {
        throw ClientModificationException(NULL_AMOUNT);
    }
    founds[_founds->getCurrency()] = move(_founds);
}

const ClientTypeSPtr &Client::getClientType() const {
    return clientType;
}

void Client::setClientType(const ClientTypeSPtr &_clientType) {
    if (_clientType == nullptr) {
        throw ClientModificationException(NULL_CLIENT_TYPE);
    }
    Client::clientType = _clientType;
}

short Client::getNumberOfCards() const {
    return cards.size();
}

bool Client::addCard(const CardSPtr &card) {
    if (card == nullptr) {
        throw ClientModificationException(NULL_CARD);
    }
    if (find(cards.begin(), cards.end(), card) != cards.end()) {
        return false;
    }
    cards.emplace_back(card);
    return true;
}

bool Client::removeCard(const CardSPtr &card) {
    if (card == nullptr) {
        throw ClientModificationException(NULL_CARD);
    }
    auto it = find(cards.begin(), cards.end(), card);
    if (it != cards.end()) {
        cards.erase(it);
        return true;
    }
    return false;
}

short Client::getNumberOfAccounts() const {
    return accounts.size();
}

bool Client::addAccount(const AccountSPtr &account) {
    if (account == nullptr) {
        throw ClientModificationException(NULL_ACCOUNT);
    }
    if (find(accounts.begin(), accounts.end(), account) != accounts.end()) {
        return false;
    }
    accounts.emplace_back(account);
    return true;
}

bool Client::removeAccount(const AccountSPtr &account) {
    if (account == nullptr) {
        throw ClientModificationException(NULL_ACCOUNT);
    }
    auto it = find(accounts.begin(), accounts.end(), account);
    if (it != accounts.end()) {
        accounts.erase(it);
        return true;
    }
    return false;
}

short Client::getNumberOfLoans() const {
    return loans.size();
}

bool Client::addLoan(const LoanSPtr &loan) {
    if (loan == nullptr) {
        throw ClientModificationException(NULL_LOAN);
    }
    if (find(loans.begin(), loans.end(), loan) != loans.end()) {
        return false;
    }
    loans.emplace_back(loan);
    return true;
}

bool Client::removeLoan(const LoanSPtr &loan) {
    if (loan == nullptr) {
        throw ClientModificationException(NULL_LOAN);
    }
    auto it = find(loans.begin(), loans.end(), loan);
    if (it != loans.end()) {
        loans.erase(it);
        return true;
    }
    return false;
}

string Client::briefClientInfo() const {
    stringstream stream;

    stream << firstName << " " << lastName << ", ID: " << personalID << ", Address: " << address->toString();
    stream << ", Registered Address: " << registeredAddress->toString() << ", Type: " << clientType->briefClientTypeInfo();

    return stream.str();
}

string Client::toString() const {
    stringstream stream;
    int i = 1;

    stream << "=========================================================\n";
    stream << "First Name:           | " << firstName << "\n";
    stream << "Last Name:            | " << lastName << "\n";
    stream << "Personal ID:          | " << personalID << "\n";
    stream << "Address:              | " << address->toString() << "\n";
    stream << "Registered Address:   | " << registeredAddress->toString() << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "Client Type:          | " << clientType->briefClientTypeInfo() << "\n";
    stream << "Cost Multiplier:      | " << clientType->getServiceCostMultiplier() << "x\n";
    stream << "Base Service Tier:    | " << clientType->getBaseServiceTier() << "\n";
    stream << "Companies:            | " << (ownerOfCompany ? "owns company(ies)" : "does not have any company") << "\n";
    stream << "Creditworthiness:     | " << creditworthiness << "\n";
    stream << "---------------------------------------------------------\n";

    stream << "Maximum Service Cost: | ";

    const vector<AmountUPtr> &maxAmounts = clientType->getMaximumAmount();
    for (const auto &a : maxAmounts) {
        stream << a->toString() << (a->getCurrency() != PLN ? "\n                      | " : "");
    }

    stream << "\n---------------------------------------------------------\n";
    stream << "Income:               | " << *income[PLN] << "\n";
    stream << "                      | " << *income[EUR] << "\n";
    stream << "                      | " << *income[GBP] << "\n";
    stream << "                      | " << *income[USD] << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "Founds:               | " << *founds[PLN] << "\n";
    stream << "                      | " << *founds[EUR] << "\n";
    stream << "                      | " << *founds[GBP] << "\n";
    stream << "                      | " << *founds[USD] << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "Accounts:             | " << getNumberOfAccounts() << "\n";

    for (const auto &a : accounts) {
        stream << "    " << i << '.' << a->briefAccountInfo() << "\n";
        i++;
    }
    i = 1;

    stream << "---------------------------------------------------------\n";
    stream << "Cards:                | " << getNumberOfCards() << "\n";

    for (const auto &c : cards) {
        stream << "    " << i << '.' << c->briefCardInfo() << "\n";
        i++;
    }
    i = 1;

    stream << "---------------------------------------------------------\n";
    stream << "Loans:                | " << getNumberOfLoans() << "\n";

    for (const auto &l : loans) {
        stream << "    " << i << '.' << l->briefLoanInfo() << "\n";
        i++;
    }
    stream << "=========================================================\n";

    return stream.str();
}
