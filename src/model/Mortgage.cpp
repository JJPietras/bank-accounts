//
// Created by student on 21.12.2019.
//

#include <memory>
#include <utility>
#include <boost/uuid/uuid_io.hpp>

#include "model/Mortgage.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Address.h"
#include "model/ClientType.h"

using namespace std;

Mortgage::Mortgage(unique_ptr<boost::local_time::local_date_time> borrowDate, AmountUPtr amount, ClientSPtr borrower) : Loan(move(borrowDate), move(amount), move(borrower)) {}

Mortgage::~Mortgage() = default;

float Mortgage::calculatePercentage() const {
    float mainFactor = 2.40f;
    float clientFactor = getBorrower()->getClientType()->calculateServiceCostCoefficient(getBorrower()->getCreditworthiness(), make_shared<Amount>(*getBorrowedAmount()));
    if (clientFactor < 0.9f && clientFactor > 0.8f) {
        clientFactor = 0.95f;
    } else if (clientFactor < 0.8f) {
        clientFactor = 0.80f;
    }
    mainFactor -= (2 - clientFactor);
    float amountFactor = 0.0f;
    if (*getBorrowedAmount() > Amount(1000000L, 0, getCurrencyType())) {
        amountFactor = 0.20f;
    } else if (*getBorrowedAmount() > Amount(100000L, 0, getCurrencyType())) {
        amountFactor = (float) ((double) getBorrowedAmount()->getMainUnit() / 10000000);
    }
    float finalFactor = ((floor(1000 * (mainFactor - amountFactor))) / 1000.0f);
    return finalFactor;
}

string Mortgage::briefLoanInfo() const {
    return "Type: Mortgage" + Loan::briefLoanInfo();
}

string Mortgage::toString() const {
    stringstream stream;

    stream << "=========================================================\n";
    stream << "Type:            | Mortgage" << "\n";
    stream << Loan::toString();
    stream << "=========================================================";

    return stream.str();
}
