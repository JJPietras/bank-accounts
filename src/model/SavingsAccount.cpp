//
// Created by student on 21.12.2019.
//

#include "model/SavingsAccount.h"
#include "model/Client.h"
#include "model/ClientType.h"
#include "model/Amount.h"

using namespace std;

SavingsAccount::SavingsAccount(AmountUPtr amount) : Account(move(amount)) {}

SavingsAccount::SavingsAccount(unique_ptr<boost::local_time::local_date_time> openingTime, AmountUPtr amount) : Account(move(openingTime), move(amount)) {}

SavingsAccount::~SavingsAccount() = default;

bool SavingsAccount::addOwner(const ClientSPtr &owner) {
    if (!getOwners().empty()) {
        return false;
    }
    return Account::addOwner(owner);
}

AmountSPtr SavingsAccount::calculateTransferCost() const {
    if (getAmount()->getMainUnit() > 100000) {
        return make_shared<Amount>(Amount(0L, 0, PLN));
    } else if (getAmount()->getMainUnit() > 50000) {
        return make_shared<Amount>(Amount(1L, 0, PLN));
    } else if (getAmount()->getMainUnit() > 10000) {
        return make_shared<Amount>(Amount(1L, 50, PLN));
    } else if (getAmount()->getMainUnit() > 1000) {
        return make_shared<Amount>(Amount(2L, 0, PLN));
    }

    return make_shared<Amount>(Amount(5L, 0, PLN));
}

string SavingsAccount::briefAccountInfo() const {
    stringstream stream;

    stream << "Type: Savings Account, Amount: " << getAmount()->toString() << ", Owner: ";
    stream << (getOwners().empty() ? "Account has no owner" : getOwners()[0]->briefClientInfo());

    return stream.str();
}

string SavingsAccount::toString() const {
    string out = Account::toString();
    auto index = out.find("Owner:") + 1;

    stringstream stream;
    stream << (getOwners().empty() ? "Account has no owner" : getOwners()[0]->briefClientInfo()) << "\n";

    return out.insert(index, stream.str());
}
