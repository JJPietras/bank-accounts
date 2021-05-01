//
// Created by student on 21.12.2019.
//

#define CURRENCIES 4

#include <boost/algorithm/string/trim.hpp>

#include "builder/ClientBuilder.h"
#include "model/ClientType.h"
#include "model/Address.h"
#include "model/Account.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

ClientBuilder::ClientBuilder() = default;

ClientBuilder::~ClientBuilder() = default;

ClientBuilderSPtr ClientBuilder::setFirstName(const string &_firstName) {
    string newFirstName = _firstName;
    boost::trim(newFirstName);
    if (newFirstName.empty()) {
        throw ClientBuilderModificationException(EMPTY_FIRST_NAME);
    }
    firstName = newFirstName;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setLastName(const string &_lastName) {
    string newLastName = _lastName;
    boost::trim(newLastName);
    if (newLastName.empty()) {
        throw ClientBuilderModificationException(EMPTY_LAST_NAME);
    }
    lastName = newLastName;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setPersonalID(const string &_personalID) {
    string newPersonalID = _personalID;
    boost::trim(newPersonalID);
    if (newPersonalID.empty()) {
        throw ClientBuilderModificationException(EMPTY_PERSONAL_ID);
    }
    personalID = newPersonalID;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setCreditworthiness(short _creditworthiness) {
    creditworthiness = _creditworthiness;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setOwnerOfCompany(bool _ownerOfCompany) {
    ownerOfCompany = _ownerOfCompany;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setRegisteredAddress(const AddressSPtr &_registeredAddress) {
    if (_registeredAddress == nullptr) {
        throw ClientBuilderModificationException(NULL_ADDRESS);
    }
    registeredAddress = _registeredAddress;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setAddress(const AddressSPtr &_address) {
    if (_address == nullptr) {
        throw ClientBuilderModificationException(NULL_ADDRESS);
    }
    address = _address;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setClientType(const ClientTypeSPtr &_clientType) {
    if (_clientType == nullptr) {
        throw ClientBuilderModificationException(NULL_CLIENT_TYPE);
    }
    clientType = _clientType;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setIncome(const AmountSPtr &_income) {
    if (_income == nullptr) {
        throw ClientBuilderModificationException(NULL_AMOUNT);
    }
    if (_income->getMainUnit() < 0) {
        throw ClientBuilderModificationException(NEGATIVE_INCOME);
    }
    income[_income->getCurrency()] = _income;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setFounds(const AmountSPtr &_founds) {
    if (_founds == nullptr) {
        throw ClientBuilderModificationException(NULL_AMOUNT);
    }
    founds[_founds->getCurrency()] = _founds;
    return make_shared<ClientBuilder>(*this);
}

ClientBuilderSPtr ClientBuilder::setAccounts(const vector<AccountSPtr> &_accounts) {
    if (_accounts.empty()) {
        throw ClientBuilderModificationException(EMPTY_COLLECTION);
    }
    for (ulong i = 0; i < _accounts.size(); i++) {
        for (ulong j = i + 1; j < _accounts.size(); j++) {
            if (_accounts[j] == _accounts[i]) {
                throw ClientBuilderModificationException(ACCOUNT_EXISTS);
            }
        }
    }
    ClientBuilder::accounts = _accounts;
    return make_shared<ClientBuilder>(*this);
}

ClientSPtr ClientBuilder::build() {
    if (firstName.empty() || lastName.empty() || personalID.empty() || registeredAddress == nullptr) {
        throw ClientBuilderBuildException(CLIENT_CRUCIAL_MISSING);
    }

    ClientSPtr client(new Client(firstName, lastName, personalID, AddressUPtr(new Address(*registeredAddress))));

    firstName = "";
    lastName = "";
    personalID = "";
    registeredAddress = nullptr;

    if (address != nullptr) {
        client->setAddress(AddressUPtr(new Address(*address)));
        address = nullptr;
    }
    if (clientType != nullptr) {
        client->setClientType(clientType);
        clientType = nullptr;
    }
    for (int i = 0; i < CURRENCIES; i++) {
        if (income[i] != nullptr) {
            client->setIncome(AmountUPtr(new Amount(*income[i])));
        }
        if (founds[i] != nullptr) {
            client->setFounds(AmountUPtr(new Amount(*founds[i])));
        }
        income[i] = nullptr;
        founds[i] = nullptr;
    }
    for (const auto &a : accounts) {
        client->addAccount(a);
    }
    client->setOwnerOfCompany(ownerOfCompany);
    client->setCreditworthiness(creditworthiness);

    creditworthiness = 0;
    ownerOfCompany = false;
    accounts.clear();

    return client;
}
