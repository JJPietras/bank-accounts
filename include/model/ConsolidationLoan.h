//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CONSOLIDATIONLOAN_H
#define BANKKONTA_CONSOLIDATIONLOAN_H


#include <vector>

#include "Loan.h"

class StandardLoan;

class ConsolidationLoan : public Loan {
private:
    std::vector<std::shared_ptr<StandardLoan>> consolidatedLoans;

    std::unique_ptr<Amount> sumAmounts(const std::vector<std::shared_ptr<StandardLoan>> &loans) const;

public:
    ConsolidationLoan(std::unique_ptr<boost::local_time::local_date_time> borrowDate, std::vector<std::shared_ptr<StandardLoan>> consolidatedLoans);

    ~ConsolidationLoan() override;

    float calculatePercentage() const override;

    std::string briefLoanInfo() const override;

    std::string toString() const override;
};

typedef std::shared_ptr<ConsolidationLoan> ConsolidationLoanSPtr;
typedef std::unique_ptr<ConsolidationLoan> ConsolidationLoanUPtr;

#endif //BANKKONTA_CONSOLIDATIONLOAN_H
