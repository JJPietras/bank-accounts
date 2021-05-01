//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_ACCOUNTMANAGER_H
#define BANKKONTA_ACCOUNTMANAGER_H


#include <memory>
#include <functional>

#include "enum/AccountRepositoriesEnum.h"

class Client;

class Amount;

class Account;

class AccountRepository;

class AccountBuilder;

class AccountManager {
private:
    std::shared_ptr<AccountRepository> closedAccounts;
    std::shared_ptr<AccountRepository> jointAccounts;
    std::shared_ptr<AccountRepository> savingsAccounts;
    std::shared_ptr<AccountRepository> currencyAccounts;

    std::shared_ptr<AccountRepository> getRepository(AccountRepositoriesEnum repository) const;

public:
    AccountManager(std::shared_ptr<AccountRepository> closedAccounts,
                   std::shared_ptr<AccountRepository> jointAccounts,
                   std::shared_ptr<AccountRepository> savingsAccounts,
                   std::shared_ptr<AccountRepository> currencyAccounts);

    ~AccountManager();

    void closeAccount(std::shared_ptr<Account> account);

    void createAccount(const std::shared_ptr<AccountBuilder> &prePreparedAccount);

    std::shared_ptr<Account> getAccount(const std::function<bool(std::shared_ptr<Account> account)> &predicate, AccountRepositoriesEnum repository);

    void makeTransfer(const std::shared_ptr<Account>& from, const std::shared_ptr<Account>& to, const std::shared_ptr<Amount>& amount);

    void addOwnerToAccount(const std::shared_ptr<Account> &account, const std::shared_ptr<Client> &owner);

    void removeOwnerFromAccount(const std::shared_ptr<Account> &account, const std::shared_ptr<Client> &owner);

    std::string accountInfo(const std::shared_ptr<Account> &account) const;
};

typedef std::shared_ptr<AccountManager> AccountManagerSPtr;
typedef std::unique_ptr<AccountManager> AccountManagerUPtr;

#endif //BANKKONTA_ACCOUNTMANAGER_H
