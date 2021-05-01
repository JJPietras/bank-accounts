//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_LOANREPOSITORY_H
#define BANKKONTA_LOANREPOSITORY_H


#include "Repository.h"

class Loan;

class LoanRepository : public Repository<Loan> {
private:
    std::vector<std::shared_ptr<Loan>> loans;

public:
    LoanRepository();

    ~LoanRepository() override;

    void add(const std::shared_ptr<Loan> &element) override;

    void remove(const std::shared_ptr<Loan> &element) override;

    std::vector<std::shared_ptr<Loan>> getAll() override;

    std::shared_ptr<Loan> find(const std::function<bool(std::shared_ptr<Loan> loan)> &predicate) override;

};

typedef std::shared_ptr<LoanRepository> LoanRepositorySPtr;
typedef std::unique_ptr<LoanRepository> LoanRepositoryUPtr;

#endif //BANKKONTA_LOANREPOSITORY_H
