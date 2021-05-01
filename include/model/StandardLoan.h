//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_STANDARDLOAN_H
#define BANKKONTA_STANDARDLOAN_H


#include "Loan.h"

class StandardLoan : public Loan {
public:
    StandardLoan(std::unique_ptr<boost::local_time::local_date_time> borrowDate, std::unique_ptr<Amount> amount, const std::shared_ptr<Client> &borrower);

    ~StandardLoan() override;

    float calculatePercentage() const override;

    std::string briefLoanInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<StandardLoan> StandardLoanSPtr;
typedef std::unique_ptr<StandardLoan> StandardLoanUPtr;

#endif //BANKKONTA_STANDARDLOAN_H
