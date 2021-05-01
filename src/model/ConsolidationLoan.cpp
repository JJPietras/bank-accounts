//
// Created by student on 21.12.2019.
//

#include <utility>

#include "model/ConsolidationLoan.h"
#include "model/StandardLoan.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "exceptions/LoanExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

ConsolidationLoan::ConsolidationLoan(unique_ptr<boost::local_time::local_date_time> borrowDate, vector<StandardLoanSPtr> consolidatedLoans)
        : Loan(move(borrowDate), sumAmounts(consolidatedLoans), consolidatedLoans.empty() ? nullptr : consolidatedLoans[0]->getBorrower()),
          consolidatedLoans(consolidatedLoans.empty() ? throw ConsolidationLoanConstructionException(NULL_LOAN) : move(consolidatedLoans)) {

    CurrencyType type = this->consolidatedLoans[0]->getCurrencyType();
    ClientSPtr client = this->consolidatedLoans[0]->getBorrower();

    for (ulong i = 0; i < this->consolidatedLoans.size(); i++) {
        if (this->consolidatedLoans[i]->getCurrencyType() != type) {
            throw ConsolidationLoanConstructionException(MULTIPLE_CURRENCIES);
        }
        if (this->consolidatedLoans[i]->getBorrower() != client) {
            throw ConsolidationLoanConstructionException(MULTIPLE_BORROWERS);
        }
        for (ulong j = i + 1; j < this->consolidatedLoans.size(); j++) {
            if (this->consolidatedLoans[i]->getUuid() == this->consolidatedLoans[j]->getUuid()) {
                throw ConsolidationLoanConstructionException(SAME_LOANS);
            }
        }
    }
    if (this->getBorrowDate() == nullptr) {
        throw ConsolidationLoanConstructionException(NULL_DATE_TIME);
    }
    Amount returnedAmount(0, 0, getCurrencyType());
    for (const auto &l : this->consolidatedLoans) {
        returnedAmount = returnedAmount + *l->getReturnedAmount();
    }
    Loan::returnMoney(AmountUPtr(new Amount(returnedAmount)));
}

ConsolidationLoan::~ConsolidationLoan() = default;

float ConsolidationLoan::calculatePercentage() const {
    float percentage = 0.0f;

    for (const auto &l : consolidatedLoans) {
        percentage += l->calculatePercentage();
    }
    percentage /= consolidatedLoans.size();

    return percentage;
}

string ConsolidationLoan::briefLoanInfo() const {
    return "Type: Consolidation Loan" + Loan::briefLoanInfo();
}

string ConsolidationLoan::toString() const {
    stringstream stream;

    stream << "=========================================================\n";
    stream << "Type:            | Consolidation Loan" << "\n";
    stream << Loan::toString();
    stream << "---------------------------------------------------------\n";
    stream << "Consolidations:  | " << consolidatedLoans.size() << "\n";

    for (ulong i = 0; i < consolidatedLoans.size(); i++) {
        stream << "    " << i + 1 << '.' << (*consolidatedLoans[i]).briefLoanInfo() << "\n";
    }

    stream << "=========================================================";

    return stream.str();
}

AmountUPtr ConsolidationLoan::sumAmounts(const vector<StandardLoanSPtr> &loans) const {
    if (loans.size() < 2) {
        throw ConsolidationLoanMethodException(TOO_FEW_LOANS);
    }

    Amount amount = Amount(0, 0, loans[0]->getCurrencyType());
    for (const auto &l : loans) {
        if (l->getCurrencyType() != amount.getCurrency()) {
            throw ConsolidationLoanMethodException(CURRENCY);
        }
        amount = amount + *l->getBorrowedAmount();
    }
    return AmountUPtr(new Amount(amount));
}
