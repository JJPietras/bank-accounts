//
// Created by student on 21.12.2019.
//

#include <utility>

#include "manager/LoanManager.h"
#include "builder/LoanBuilder.h"
#include "repository/LoanRepository.h"
#include "model/Loan.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/ClientType.h"
#include "exceptions/LoanExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

LoanManager::LoanManager(LoanRepositorySPtr currentLoans, LoanRepositorySPtr finalizedLoans)
        : currentLoans(move(currentLoans)), finalizedLoans(move(finalizedLoans)) {
    if (this->currentLoans == nullptr || this->finalizedLoans == nullptr) {
        throw LoanManagerConstructionException(NULL_REPO);
    }
}

LoanManager::~LoanManager() = default;

void LoanManager::createLoan(const LoanBuilderSPtr &prePreparedLoan) {
    if (prePreparedLoan == nullptr) {
        throw LoanManagerMethodException(NULL_BUILDER);
    }
    LoanSPtr loan;
    try {
        loan = prePreparedLoan->build();
    }
    catch (LoanException &) {
        throw LoanManagerException(BUILDER_EXCEPTION);
    }

    ClientSPtr client = loan->getBorrower();
    CurrencyType currency = loan->getBorrowedAmount()->getCurrency();
    const vector<LoanSPtr> &loans = currentLoans->getAll();
    bool paid = true;

    if (*loan->getLoanCost() > *client->getClientType()->getMaximumAmount()[currency]) {
        throw LoanManagerMethodException(TOO_EXPENSIVE);
    }
    if (calculateTier(loan) > client->getClientType()->calculateServiceLevel(client->getCreditworthiness())) {
        throw LoanManagerMethodException(TOO_HIGH_TIER);
    }

    for (const auto &l : loans) {
        if (l->getBorrower() == client) {
            if (l->getPaymentDate() != nullptr) {
                client->setCreditworthiness((short) (client->getCreditworthiness() + 2));
                markAsFinalized(l);
                continue;
            }
            client->setFounds(AmountUPtr(new Amount(*client->getFounds()[currency] - (*l->getLoanCost() - *l->getReturnedAmount()))));
            client->setCreditworthiness((short) (client->getCreditworthiness() - 5));
            paid = false;
        }
    }
    if (paid) {
        client->setCreditworthiness((short) (client->getCreditworthiness() + 10));
    }

    client->addLoan(loan);
    currentLoans->add(loan);
}

LoanSPtr LoanManager::getLoan(const function<bool(LoanSPtr loan)> &predicate, LoanRepositoriesEnum repository) {
    return getRepository(repository)->find(predicate);
}

void LoanManager::markAsFinalized(const LoanSPtr &loan) {
    if (loan == nullptr) {
        throw LoanManagerMethodException(NULL_LOAN);
    }
    if (currentLoans->find([&loan](const LoanSPtr &otherLoan) { return loan->getUuid() == otherLoan->getUuid(); }) != nullptr) {
        currentLoans->remove(loan);
    } else {
        throw LoanManagerMethodException(ELEMENT_NOT_REMOVED);
    }
    loan->getBorrower()->removeLoan(loan);
    finalizedLoans->add(loan);
}

void LoanManager::returnMoney(const LoanSPtr &loan, const AmountSPtr &amount) {
    if (loan == nullptr) {
        throw LoanManagerMethodException(NULL_LOAN);
    }
    if (loan->getPaymentDate() != nullptr) {
        throw LoanManagerMethodException(REPAID);
    }
    if (currentLoans->find([&loan](const LoanSPtr &otherLoan) { return loan->getUuid() == otherLoan->getUuid(); }) == nullptr) {
        throw LoanManagerMethodException(ELEMENT_NOT_REMOVED);
    }
    loan->returnMoney(amount);
}

string LoanManager::loanInfo(const LoanSPtr &loan) const {
    if (loan == nullptr) {
        throw LoanManagerMethodException(NULL_LOAN);
    }
    return loan->toString();
}

short LoanManager::calculateTier(const LoanSPtr &loan) const {
    long number = loan->getBorrowedAmount()->getMainUnit();
    short zeros = 1;
    short currencyTier = 0;

    while (number > 10) {
        number /= 10;
        zeros *= 2;
    }

    switch (loan->getCurrencyType()) {
        case EUR:
            currencyTier = 4;
            break;
        case USD:
            currencyTier = 3;
            break;
        case GBP:
            currencyTier = 5;
            break;
        case PLN:
            currencyTier = 1;
            break;
    }
    return (short) (zeros + currencyTier);
}

LoanRepositorySPtr LoanManager::getRepository(LoanRepositoriesEnum repository) const {
    LoanRepositorySPtr repo;
    switch (repository) {
        case CurrentLoansRepo:
            repo = currentLoans;
            break;
        case FinalizedLoansRepo:
            repo = finalizedLoans;
            break;
    }
    return repo;
}
