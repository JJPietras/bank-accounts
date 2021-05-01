//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_MORTGAGE_H
#define BANKKONTA_MORTGAGE_H


#include "Loan.h"

class Mortgage : public Loan {
public:
    Mortgage(std::unique_ptr<boost::local_time::local_date_time> borrowDate, std::unique_ptr<Amount> amount, std::shared_ptr<Client> borrower);

    ~Mortgage() override;

    float calculatePercentage() const override;

    std::string briefLoanInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<Mortgage> MortgageSPtr;
typedef std::unique_ptr<Mortgage> MortgageUPtr;

#endif //BANKKONTA_MORTGAGE_H
