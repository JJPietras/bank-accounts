//
// Created by student on 21.12.2019.
//

#include "builder/LoanBuilder.h"
#include "model/Mortgage.h"
#include "model/StandardLoan.h"
#include "model/ConsolidationLoan.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/Loan.h"
#include "exceptions/LoanExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

LoanBuilder::LoanBuilder() = default;

LoanBuilder::~LoanBuilder() = default;

LoanBuilderSPtr LoanBuilder::setBorrowedAmount(const AmountSPtr &_borrowedAmount) {
    if (_borrowedAmount == nullptr) {
        throw LoanBuilderModificationException(NULL_AMOUNT);
    }
    if (_borrowedAmount->getMainUnit() < 10L) {
        throw LoanBuilderModificationException(WRONG_BORROW_AMOUNT);
    }
    borrowedAmount = _borrowedAmount;
    return make_shared<LoanBuilder>(*this);
}

LoanBuilderSPtr LoanBuilder::setReturnedAmount(const AmountSPtr &_returnedAmount) {
    if (_returnedAmount == nullptr) {
        throw LoanBuilderModificationException(NULL_AMOUNT);
    }
    if (_returnedAmount->getMainUnit() < 0L || (_returnedAmount->getMainUnit() == 0 && _returnedAmount->getFractionalUnit() < 0)) {
        throw LoanBuilderModificationException(NEGATIVE_AMOUNT);
    }
    returnedAmount = _returnedAmount;
    return make_shared<LoanBuilder>(*this);
}

LoanBuilderSPtr LoanBuilder::setLoanType(const shared_ptr<LoanEnum> &_loanType) {
    if (_loanType == nullptr) {
        throw LoanBuilderModificationException(NULL_LOAN_TYPE);
    }
    loanType = _loanType;
    return make_shared<LoanBuilder>(*this);
}

LoanBuilderSPtr LoanBuilder::setBorrowDate(const shared_ptr<boost::local_time::local_date_time> &_borrowDate) {
    if (_borrowDate == nullptr) {
        throw LoanBuilderModificationException(NULL_DATE_TIME);
    }
    borrowDate = _borrowDate;
    return make_shared<LoanBuilder>(*this);
}

LoanBuilderSPtr LoanBuilder::setConsolidatedLoans(const vector<StandardLoanSPtr> &_consolidatedLoans) {
    if (_consolidatedLoans.size() < 2) {
        throw LoanBuilderModificationException(TOO_FEW_LOANS);
    }
    for (ulong i = 0; i < _consolidatedLoans.size(); i++) {
        for (ulong j = i + 1; j < _consolidatedLoans.size(); j++) {
            if (_consolidatedLoans[j] == _consolidatedLoans[i]) {
                throw LoanBuilderModificationException(LOAN_EXISTS);
            }
        }
    }
    consolidatedLoans = _consolidatedLoans;
    return make_shared<LoanBuilder>(*this);
}

LoanBuilderSPtr LoanBuilder::setBorrower(const ClientSPtr &_borrower) {
    if (_borrower == nullptr) {
        throw LoanBuilderModificationException(NULL_CLIENT);
    }
    borrower = _borrower;
    return make_shared<LoanBuilder>(*this);
}

LoanSPtr LoanBuilder::build() {
    if (borrowDate == nullptr || borrowedAmount == nullptr || borrower == nullptr || loanType == nullptr) {
        throw LoanBuilderBuildException(LOAN_CRUCIAL_MISSING);
    }

    LoanSPtr loan;

    unique_ptr<boost::local_time::local_date_time> date(new boost::local_time::local_date_time(*borrowDate));
    AmountUPtr amount(new Amount(*borrowedAmount));

    switch (*loanType) {
        case StandardLoanType:
            loan.reset(new StandardLoan(move(date), move(amount), borrower));
            break;
        case ConsolidationLoanType:
            if (consolidatedLoans.empty()) {
                throw LoanBuilderBuildException(EMPTY_CONSOLIDATED_LOANS);
            }
            loan.reset(new ConsolidationLoan(move(date), consolidatedLoans));
            consolidatedLoans.clear();
            break;
        case MortgageType:
            loan.reset(new Mortgage(move(date), move(amount), borrower));
            break;
    }

    borrowDate = nullptr;
    borrower = nullptr;
    loanType = nullptr;

    if (returnedAmount != nullptr) {
        if (borrowedAmount->getCurrency() != returnedAmount->getCurrency()) {
            throw LoanBuilderBuildException(CURRENCY);
        }
        loan->returnMoney(returnedAmount);
        returnedAmount = nullptr;
    }
    borrowedAmount = nullptr;

    return loan;
}
