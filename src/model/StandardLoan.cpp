//
// Created by student on 21.12.2019.
//

#include "model/StandardLoan.h"
#include "model/ClientType.h"
#include "model/Amount.h"
#include "model/Client.h"

using namespace std;

StandardLoan::StandardLoan(unique_ptr<boost::local_time::local_date_time> borrowDate, AmountUPtr amount, const ClientSPtr &borrower) : Loan(move(borrowDate), move(amount), borrower) {}

StandardLoan::~StandardLoan() = default;

float StandardLoan::calculatePercentage() const {
    float mainFactor = 1.0f;
    float clientFactor = 1.0f;
    clientFactor -= getBorrower()->getClientType()->calculateServiceCostCoefficient(getBorrower()->getCreditworthiness(), make_shared<Amount>(*getBorrowedAmount()));
    mainFactor += clientFactor;
    float amountFactor;
    if (*getBorrowedAmount() < Amount(100000L, 0, getCurrencyType())) {
        amountFactor = 0.10f;
    } else {
        amountFactor = 0.20f;
    }
    float finalFactor = ((floor(1000 * (mainFactor + amountFactor))) / 1000.0f);
    return finalFactor;
}

string StandardLoan::briefLoanInfo() const {
    return "Type: Standard Loan" + Loan::briefLoanInfo();
}

string StandardLoan::toString() const {
    stringstream stream;

    stream << "=========================================================\n";
    stream << "Type:            | Standard Loan" << "\n";
    stream << Loan::toString();
    stream << "=========================================================";

    return stream.str();
}
