//
// Created by student on 21.12.2019.
//

#include "model/CurrencyAccount.h"
#include "model/Client.h"
#include "model/Amount.h"

using namespace std;

CurrencyAccount::CurrencyAccount(AmountUPtr amount) : Account(move(amount)) {}

CurrencyAccount::CurrencyAccount(unique_ptr<boost::local_time::local_date_time> openingTime, AmountUPtr amount) : Account(move(openingTime), move(amount)) {}

CurrencyAccount::~CurrencyAccount() = default;

bool CurrencyAccount::addOwner(const ClientSPtr &owner) {
    if (!getOwners().empty()) {
        return false;
    }
    return Account::addOwner(owner);
}

AmountSPtr CurrencyAccount::calculateTransferCost() const {
    float currencyImpact = 0.0f;
    float factor = 1.0f;
    long mainUnit = getAmount()->getMainUnit();
    switch (getCurrency()) {
        case EUR:
            currencyImpact = 0.2f;
            break;
        case USD:
            currencyImpact = 0.15f;
            break;
        case GBP:
            currencyImpact = 0.25f;
            break;
        case PLN:
            currencyImpact = 0.05f;
            break;
    }
    while (mainUnit > 100L) {
        mainUnit /= 10L;
        factor /= 2.0f;
    }
    factor += currencyImpact;
    return make_shared<Amount>(Amount(5L, 0, getAmount()->getCurrency()) * factor);
}

string CurrencyAccount::briefAccountInfo() const {
    string owner = getOwners()[0]->getFirstName() + " " + getOwners()[0]->getLastName();
    return "Type: Currency Account, Amount: " + getAmount()->toString() + ", Currency: " + Amount::currencyToString(getCurrency(), false) + ", Owner: " + owner;
}

string CurrencyAccount::toString() const {
    string out = Account::toString();
    auto index = out.find("Owner:") + 1;

    stringstream stream;
    stream << (getOwners().empty() ? "Account has no owner" : getOwners()[0]->briefClientInfo()) << "\n";

    return out.insert(index, stream.str());
}
