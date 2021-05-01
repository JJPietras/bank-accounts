//
// Created by student on 21.12.2019.
//

#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/clamp.hpp>

#include "model/JointAccount.h"
#include "model/ClientType.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Card.h"
#include "exceptions/AccountExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

JointAccount::JointAccount(AmountUPtr amount, short maxOwners) : Account(move(amount)), maxOwners(maxOwners) {
    if (getAmount()->getCurrency() != PLN) {
        throw JointAccountConstructionException(CURRENCY);
    }
    if (this->maxOwners < 2) {
        throw JointAccountConstructionException(LESS_THAN_TWO);
    }
}

JointAccount::JointAccount(unique_ptr<boost::local_time::local_date_time> openingTime, AmountUPtr amount, short maxOwners)
        : Account(move(openingTime), move(amount)), maxOwners(maxOwners) {
    if (getAmount()->getCurrency() != PLN) {
        throw JointAccountConstructionException(CURRENCY);
    }
    if (this->maxOwners < 2) {
        throw JointAccountConstructionException(LESS_THAN_TWO);
    }
}

JointAccount::~JointAccount() = default;

bool JointAccount::addOwner(const ClientSPtr &owner) {
    if (owner == nullptr) {
        throw JointAccountMethodException(NULL_CLIENT);
    }
    if (getOwners().size() < (ulong) maxOwners) {
        return Account::addOwner(owner);
    }
    return false;
}

AmountSPtr JointAccount::calculateTransferCost() const {
    float clientsFactor = getOwners().empty() ? 2.0f : 1.0f;
    short ownerCount = getOwners().empty() ? 1 : getOwners().size();

    clientsFactor /= (float) ownerCount;
    clientsFactor *= 2;
    clientsFactor = boost::algorithm::clamp(clientsFactor, 0.5f, 2.0f);

    return make_shared<Amount>(Amount(2L, 0, PLN) * clientsFactor);
}

string JointAccount::briefAccountInfo() const {
    return "Type: Joint Account, Amount: " + getAmount()->toString() + ", Max Owners: " + to_string(maxOwners) + ", Number of Owners: " + to_string(getOwners().size());
}

string JointAccount::toString() const {
    string out = Account::toString();
    auto index = out.find("Owner:");
    auto owners = getOwners();

    stringstream stream;
    stream << "Owners:         | " << owners.size() << "\n";

    int i = 0;
    for (const auto &o : owners) {
        stream << "    " << i << '.' << o->briefClientInfo() << "\n";
        i++;
    }

    return out.replace(index, 5UL, stream.str());
}
