//
// Created by student on 21.12.2019.
//

#include <memory>
#include <utility>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

#include "model/Loan.h"
#include "model/Amount.h"
#include "model/Address.h"
#include "model/Client.h"
#include "exceptions/LoanExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

Loan::Loan(unique_ptr<boost::local_time::local_date_time> borrowDate, AmountUPtr amount, ClientSPtr borrower)
        : borrowDate(move(borrowDate)), borrowedAmount(move(amount)), borrower(move(borrower)) {
    if (this->borrowDate == nullptr) {
        throw LoanConstructionException(NULL_DATE_TIME);
    }
    if (this->borrowDate->is_not_a_date_time()) {
        throw LoanConstructionException(NOT_DATE_TIME);
    }
    if (this->borrowedAmount == nullptr) {
        throw LoanConstructionException(NULL_AMOUNT);
    }
    if (this->borrowedAmount->getMainUnit() < 10L) {
        throw LoanConstructionException(WRONG_BORROW_AMOUNT);
    }
    if (this->borrower == nullptr) {
        throw LoanConstructionException(NULL_OWNER);
    }
    paymentDate = nullptr;
    returnedAmount = AmountUPtr(new Amount(0L, 0, getBorrowedAmount()->getCurrency()));
    uuid = unique_ptr<boost::uuids::uuid>(new boost::uuids::uuid(boost::uuids::random_generator()()));
}

Loan::~Loan() = default;

CurrencyType Loan::getCurrencyType() const {
    return borrowedAmount->getCurrency();
}

const unique_ptr<boost::uuids::uuid> &Loan::getUuid() const {
    return uuid;
}

const unique_ptr<boost::local_time::local_date_time> &Loan::getBorrowDate() const {
    return borrowDate;
}

const unique_ptr<boost::local_time::local_date_time> &Loan::getPaymentDate() const {
    return paymentDate;
}

const AmountUPtr &Loan::getBorrowedAmount() const {
    return borrowedAmount;
}

const AmountUPtr &Loan::getReturnedAmount() const {
    return returnedAmount;
}

const ClientSPtr &Loan::getBorrower() const {
    return borrower;
}

AmountSPtr Loan::getLoanCost() const {
    return make_shared<Amount>(*borrowedAmount * calculatePercentage());
}

bool Loan::returnMoney(const AmountSPtr &amount) {
    if (amount == nullptr) {
        throw LoanModificationException(NULL_AMOUNT);
    }
    if (amount->getCurrency() != getCurrencyType()) {
        throw LoanModificationException(CURRENCY);
    }
    if (*amount < Amount(0L, 0, getCurrencyType())) {
        throw LoanModificationException(NEGATIVE_AMOUNT);
    }
    Amount maxAmount = *borrowedAmount * calculatePercentage();
    if (*returnedAmount + *amount > maxAmount || *returnedAmount + *amount == maxAmount) {
        if (paymentDate == nullptr) {
            paymentDate.reset(new boost::local_time::local_date_time(boost::posix_time::second_clock::local_time(), borrowDate->zone()));
            if (*returnedAmount + *amount > maxAmount) {
                borrower->setFounds(AmountUPtr(new Amount(*borrower->getFounds()[amount->getCurrency()] + *returnedAmount + *amount - maxAmount)));
            } else {
                *returnedAmount = *returnedAmount + *amount;
            }
        } else {
            borrower->setFounds(AmountUPtr(new Amount(*borrower->getFounds()[amount->getCurrency()] + *amount)));
        }
        return true;
    }
    *returnedAmount = *returnedAmount + *amount;
    return false;
}

string Loan::briefLoanInfo() const {
    string out = ", Amount: " + getBorrowedAmount()->toString() + ", Returned: " + getReturnedAmount()->toString();
    return out + ", Borrower: " + getBorrower()->getFirstName() + " " + getBorrower()->getLastName() + ", Interest Rate: " + to_string((int) (ceil(calculatePercentage() * 100)) % 100) + "%";
}

string Loan::toString() const {
    stringstream stream;

    stream << "Currency         | " << Amount::currencyToString(getCurrencyType(), false) << "\n";
    stream << "Amount:          | " << *getBorrowedAmount() << "\n";
    stream << "Returned Amount: | " << *getReturnedAmount() << "\n";
    stream << "Interest Rate:   | " << to_string((int) (ceil(calculatePercentage() * 100)) % 100) << "%\n";
    stream << "Borrowing Date:  | " << getBorrowDate()->to_string() << "\n";
    stream << "Repayment Date:  | " << (getPaymentDate() == nullptr ? "Loan is not repaid yet" : getPaymentDate()->to_string()) << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "Borrower:\n" << getBorrower()->briefClientInfo() << "\n";
    stream << "---------------------------------------------------------\n";
    stream << "UUID:            | " << boost::uuids::to_string(*getUuid()) << "\n";

    return stream.str();
}
