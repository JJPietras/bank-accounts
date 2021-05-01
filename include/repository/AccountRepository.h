//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_ACCOUNTREPOSITORY_H
#define BANKKONTA_ACCOUNTREPOSITORY_H


#include "Repository.h"

class Account;

class AccountRepository : public Repository<Account> {
private:
    std::vector<std::shared_ptr<Account>> accounts;

public:
    AccountRepository();

    ~AccountRepository() override;

    void add(const std::shared_ptr<Account> &element) override;

    void remove(const std::shared_ptr<Account> &element) override;

    std::vector<std::shared_ptr<Account>> getAll() override;

    std::shared_ptr<Account> find(const std::function<bool(std::shared_ptr<Account> account)> &predicate) override;

};

typedef std::shared_ptr<AccountRepository> AccountRepositorySPtr;
typedef std::unique_ptr<AccountRepository> AccountRepositoryUPtr;

#endif //BANKKONTA_ACCOUNTREPOSITORY_H
