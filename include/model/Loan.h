//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_LOAN_H
#define BANKKONTA_LOAN_H


#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/date_time/local_time/local_date_time.hpp>

#include "enum/CurrencyType.h"

class Client;

class Amount;

class Loan {
private:
    std::unique_ptr<boost::uuids::uuid> uuid;
    std::unique_ptr<boost::local_time::local_date_time> borrowDate;
    std::unique_ptr<boost::local_time::local_date_time> paymentDate;
    std::unique_ptr<Amount> borrowedAmount;
    std::unique_ptr<Amount> returnedAmount;
    std::shared_ptr<Client> borrower;

public:
    Loan(std::unique_ptr<boost::local_time::local_date_time> borrowDate, std::unique_ptr<Amount> amount, std::shared_ptr<Client> borrower);

    virtual ~Loan();

    CurrencyType getCurrencyType() const;

    const std::unique_ptr<boost::uuids::uuid> &getUuid() const;

    const std::unique_ptr<boost::local_time::local_date_time> &getBorrowDate() const;

    const std::unique_ptr<boost::local_time::local_date_time> &getPaymentDate() const;

    const std::unique_ptr<Amount> &getBorrowedAmount() const;

    const std::unique_ptr<Amount> &getReturnedAmount() const;

    const std::shared_ptr<Client> &getBorrower() const;

    std::shared_ptr<Amount> getLoanCost() const;

    bool returnMoney(const std::shared_ptr<Amount> &amount);

    virtual float calculatePercentage() const = 0;

    virtual std::string briefLoanInfo() const;

    virtual std::string toString() const;

};

typedef std::shared_ptr<Loan> LoanSPtr;
typedef std::unique_ptr<Loan> LoanUPtr;

#endif //BANKKONTA_LOAN_H
