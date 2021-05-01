//
// Created by student on 21.12.2019.
//

#include "model/Loan.h"
#include "repository/LoanRepository.h"
#include "exceptions/LoanExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

LoanRepository::LoanRepository() = default;

LoanRepository::~LoanRepository() = default;

void LoanRepository::add(const LoanSPtr &element) {
    if (element == nullptr) {
        throw LoanRepositoryException(NULL_LOAN);
    }
    if (loans.empty()) {
        loans.emplace_back(element);
        return;
    }
    auto possibleExistingLoan = find([&element](const LoanSPtr &loan) { return *element->getUuid() == *loan->getUuid(); });
    if (possibleExistingLoan == nullptr) {
        loans.emplace_back(element);
    } else {
        throw LoanRepositoryException(LOAN_EXISTS);
    }
}

void LoanRepository::remove(const LoanSPtr &element) {
    if (element == nullptr) {
        throw LoanRepositoryException(NULL_LOAN);
    }
    if (loans.empty()) {
        throw LoanRepositoryException(EMPTY_COLLECTION);
    }

    auto loanIterator = loans.begin();
    for (; loanIterator != loans.end(); loanIterator++) {
        if (*(*loanIterator)->getUuid() == *element->getUuid()) {
            loans.erase(loanIterator);
            return;
        }
    }
    throw LoanRepositoryException(LOAN_EXISTS);
}

vector<LoanSPtr> LoanRepository::getAll() {
    return loans;
}

LoanSPtr LoanRepository::find(const function<bool(LoanSPtr loan)> &predicate) {
    auto found = find_if(loans.begin(), loans.end(), predicate);
    if (found == loans.end()) {
        return nullptr;
    }
    return *found;
}
