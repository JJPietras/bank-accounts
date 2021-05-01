//
// Created by student on 21.12.2019.
//

#include <boost/date_time/local_time/local_time.hpp>

#include "builder/AccountBuilder.h"
#include "model/CurrencyAccount.h"
#include "model/SavingsAccount.h"
#include "model/JointAccount.h"
#include "model/Client.h"
#include "model/Card.h"
#include "model/Amount.h"
#include "model/Account.h"
#include "exceptions/AccountExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

AccountBuilder::AccountBuilder() = default;

AccountBuilder::~AccountBuilder() = default;

AccountBuilderSPtr AccountBuilder::setMaxOwners(const shared_ptr<short> &_maxOwners) {
    if (_maxOwners == nullptr) {
        throw AccountBuilderModificationException(NULL_MAX_OWNERS);
    }
    if (*_maxOwners < 2) {
        throw AccountBuilderModificationException(ONE_LESS_OWNERS);
    }
    return make_shared<AccountBuilder>(*this);
}

AccountBuilderSPtr AccountBuilder::setAmount(const AmountSPtr &_amount) {
    if (_amount == nullptr) {
        throw AccountBuilderModificationException(NULL_AMOUNT);
    }
    if (*_amount < Amount(0L, 0, _amount->getCurrency())) {
        throw AccountBuilderModificationException(NEGATIVE_AMOUNT);
    }
    amount = _amount;
    return make_shared<AccountBuilder>(*this);
}

AccountBuilderSPtr AccountBuilder::setOpeningTime(const shared_ptr<boost::local_time::local_date_time> &_openingTime) {
    if (_openingTime == nullptr) {
        throw AccountBuilderModificationException(NULL_DATE_TIME);
    }
    openingTime = _openingTime;
    return make_shared<AccountBuilder>(*this);
}

AccountBuilderSPtr AccountBuilder::setOwners(const vector<ClientSPtr> &_owners) {
    if (_owners.empty()) {
        throw AccountBuilderModificationException(EMPTY_COLLECTION);
    }
    for (ulong i = 0; i < _owners.size(); i++) {
        for (ulong j = i + 1; j < _owners.size(); j++) {
            if (_owners[j] == _owners[i]) {
                throw AccountBuilderModificationException(CLIENT_EXISTS);
            }
        }
    }
    owners = _owners;
    return make_shared<AccountBuilder>(*this);
}

AccountBuilderSPtr AccountBuilder::setAccountType(const shared_ptr<AccountEnum> &_accountType) {
    if (_accountType == nullptr) {
        throw AccountBuilderModificationException(NULL_ACCOUNT_TYPE);
    }
    accountType = _accountType;
    return make_shared<AccountBuilder>(*this);
}

AccountSPtr AccountBuilder::build() {
    if (amount == nullptr || accountType == nullptr) {
        throw AccountBuilderBuildException(ACCOUNT_CRUCIAL_MISSING);
    }
    if ((owners.size() > 1 && *accountType != JointAccountType) || (maxOwners != nullptr && owners.size() > (ulong) *maxOwners && *accountType == JointAccountType)) {
        throw AccountBuilderBuildException(TOO_MANY_OWNERS);
    }
    if (openingTime == nullptr) {
        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        openingTime.reset(new boost::local_time::local_date_time(time, zone));
    }

    AccountSPtr account;
    AmountUPtr amountUnique(new Amount(*amount));
    unique_ptr<boost::local_time::local_date_time> time(new boost::local_time::local_date_time(*openingTime));

    switch (*accountType) {
        case CurrencyAccountType:
            account.reset(new CurrencyAccount(move(time), move(amountUnique)));
            break;
        case SavingsAccountType:
            if (amount->getCurrency() != PLN) {
                throw AccountBuilderBuildException(WRONG_CURRENCY);
            }
            account.reset(new SavingsAccount(move(time), move(amountUnique)));
            break;
        case JointAccountType:
            if (amount->getCurrency() != PLN) {
                throw AccountBuilderBuildException(WRONG_CURRENCY);
            }
            if (maxOwners == nullptr) {
                throw AccountBuilderBuildException(NULL_MAX_OWNERS);
            }
            account.reset(new JointAccount(move(time), move(amountUnique), *maxOwners));
            maxOwners = nullptr;
            break;
    }
    if (!owners.empty()) {
        for (const auto &o : owners) {
            account->addOwner(o);
        }
        owners.clear();
    }

    amount = nullptr;
    openingTime = nullptr;

    return account;
}
