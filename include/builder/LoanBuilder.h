//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_LOANBUILDER_H
#define BANKKONTA_LOANBUILDER_H


#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/date_time/local_time/local_date_time.hpp>

#include "enum/CurrencyType.h"
#include "enum/LoanEnum.h"

class Amount;

class StandardLoan;

class Loan;

class Client;

class LoanBuilder {
private:
    std::shared_ptr<Amount> borrowedAmount = nullptr;
    std::shared_ptr<Amount> returnedAmount = nullptr;
    std::shared_ptr<LoanEnum> loanType = nullptr;
    std::shared_ptr<boost::local_time::local_date_time> borrowDate = nullptr;
    std::vector<std::shared_ptr<StandardLoan>> consolidatedLoans;
    std::shared_ptr<Client> borrower = nullptr;

public:
    LoanBuilder();

    ~LoanBuilder();

    std::shared_ptr<LoanBuilder> setBorrowedAmount(const std::shared_ptr<Amount> &_borrowedAmount);

    std::shared_ptr<LoanBuilder> setReturnedAmount(const std::shared_ptr<Amount> &_returnedAmount);

    std::shared_ptr<LoanBuilder> setLoanType(const std::shared_ptr<LoanEnum> &_loanType);

    std::shared_ptr<LoanBuilder> setBorrowDate(const std::shared_ptr<boost::local_time::local_date_time> &_borrowDate);

    std::shared_ptr<LoanBuilder> setConsolidatedLoans(const std::vector<std::shared_ptr<StandardLoan>> &_consolidatedLoans);

    std::shared_ptr<LoanBuilder> setBorrower(const std::shared_ptr<Client> &_borrower);

    std::shared_ptr<Loan> build();

};

typedef std::shared_ptr<LoanBuilder> LoanBuilderSPtr;
typedef std::unique_ptr<LoanBuilder> LoanBuilderUPtr;

#endif //BANKKONTA_LOANBUILDER_H
