//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_LOANMANAGER_H
#define BANKKONTA_LOANMANAGER_H


#include <memory>
#include <functional>

#include "enum/LoanRepositoriesEnum.h"

class Amount;

class Loan;

class LoanBuilder;

class LoanRepository;

class LoanManager {
private:
    std::shared_ptr<LoanRepository> currentLoans;
    std::shared_ptr<LoanRepository> finalizedLoans;

    short calculateTier(const std::shared_ptr<Loan> &loan) const;

    std::shared_ptr<LoanRepository> getRepository(LoanRepositoriesEnum repository) const;

public:
    LoanManager(std::shared_ptr<LoanRepository> currentLoans, std::shared_ptr<LoanRepository> finalizedLoans);

    ~LoanManager();

    void createLoan(const std::shared_ptr<LoanBuilder> &prePreparedLoan);

    std::shared_ptr<Loan> getLoan(const std::function<bool(std::shared_ptr<Loan> loan)> &predicate, LoanRepositoriesEnum repository);

    void markAsFinalized(const std::shared_ptr<Loan> &loan);

    void returnMoney(const std::shared_ptr<Loan> &loan, const std::shared_ptr<Amount> &amount);

    std::string loanInfo(const std::shared_ptr<Loan> &loan) const;

};

typedef std::shared_ptr<LoanManager> LoanManagerSPtr;
typedef std::unique_ptr<LoanManager> LoanManagerUPtr;

#endif //BANKKONTA_LOANMANAGER_H
