//
// Created by student on 21.12.2019.
//

#include <utility>

#include "manager/AccountManager.h"
#include "repository/AccountRepository.h"
#include "builder/AccountBuilder.h"
#include "model/Account.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/SavingsAccount.h"
#include "model/CurrencyAccount.h"
#include "model/JointAccount.h"
#include "exceptions/AccountExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

AccountManager::AccountManager(AccountRepositorySPtr closedAccounts, AccountRepositorySPtr jointAccounts, AccountRepositorySPtr savingsAccounts, AccountRepositorySPtr currencyAccounts)
        : closedAccounts(move(closedAccounts)), jointAccounts(move(jointAccounts)), savingsAccounts(move(savingsAccounts)), currencyAccounts(move(currencyAccounts)) {
    if (this->closedAccounts == nullptr || this->jointAccounts == nullptr || this->savingsAccounts == nullptr || this->currencyAccounts == nullptr) {
        throw AccountManagerConstructionException(NULL_REPO);
    }
}

AccountManager::~AccountManager() = default;

void AccountManager::closeAccount(AccountSPtr account) {
    if (account == nullptr) {
        throw AccountManagerMethodException(NULL_ACCOUNT);
    }
    AccountRepositorySPtr repos[]{jointAccounts, savingsAccounts, currencyAccounts};
    bool removed = false;
    for (const auto &r : repos) {
        if (r->find([&account](const AccountSPtr &otherAccount) { return account->getUuid() == otherAccount->getUuid(); }) != nullptr) {
            r->remove(account);
            removed = true;
            break;
        }
    }
    if (!removed) {
        throw AccountManagerMethodException(ELEMENT_NOT_REMOVED);
    }
    if (!account->getOwners().empty()) {
        ulong clientCount = account->getOwners().size();
        Amount dividedAmount = *account->getAmount() * (1.0 / clientCount);
        for (const auto &o : account->getOwners()) {
            o->setFounds(AmountUPtr(new Amount(*o->getFounds()[dividedAmount.getCurrency()] + dividedAmount)));
            o->removeAccount(account);
        }
        account->setAmount(AmountUPtr(new Amount(0L, 0, dividedAmount.getCurrency())));
    }

    closedAccounts->add(account);
}

void AccountManager::createAccount(const AccountBuilderSPtr &prePreparedAccount) {
    if (prePreparedAccount == nullptr) {
        throw AccountManagerMethodException(NULL_BUILDER);
    }
    AccountSPtr account;
    try {
        account = prePreparedAccount->build();
    }
    catch (AccountException &) {
        throw AccountManagerException(BUILDER_EXCEPTION);
    }

    const vector<ClientSPtr> &owners = account->getOwners();

    if (!owners.empty()) {
        for (const auto &l : owners) {
            l->addAccount(account);
        }
    }

    if (dynamic_cast<SavingsAccount *>(account.get()) != nullptr) {
        savingsAccounts->add(account);
    } else if (dynamic_cast<CurrencyAccount *>(account.get()) != nullptr) {
        currencyAccounts->add(account);
    } else {
        jointAccounts->add(account);
    }
}

AccountSPtr AccountManager::getAccount(const function<bool(AccountSPtr account)> &predicate, AccountRepositoriesEnum repository) {
    return getRepository(repository)->find(predicate);
}

void AccountManager::makeTransfer(const AccountSPtr &from, const AccountSPtr &to, const AmountSPtr &amount) {
    if (from == nullptr) {
        throw AccountManagerMethodException(NULL_ACCOUNT);
    }
    if (to == nullptr) {
        throw AccountManagerMethodException(NULL_ACCOUNT);
    }
    if (amount == nullptr) {
        throw AccountManagerMethodException(NULL_AMOUNT);
    }
    if (*amount < Amount(0L, 1, amount->getCurrency())) {
        throw AccountManagerMethodException(NEGATIVE_AMOUNT);
    }
    if (from->getCurrency() != to->getCurrency() || from->getCurrency() != amount->getCurrency()) {
        throw AccountManagerMethodException(MULTIPLE_CURRENCIES);
    }

    Amount fullCost = *amount + *from->calculateTransferCost();

    if (*from->getAmount() == fullCost || *from->getAmount() > fullCost) {
        from->setAmount(AmountUPtr(new Amount(*from->getAmount() - fullCost)));
        to->setAmount(AmountUPtr(new Amount(*to->getAmount() + *amount)));
    } else if (*from->getAmount() > *amount || *from->getAmount() == *amount) {
        Amount debit = fullCost - *amount;
        from->setAmount(AmountUPtr(new Amount(0L, 0, from->getCurrency())));
        to->setAmount(AmountUPtr(new Amount(*to->getAmount() + *amount)));
        uint clientCount = from->getOwners().size();
        Amount individualDebit = debit * (1.0 / clientCount);
        for (const auto &o : from->getOwners()) {
            o->setFounds(AmountUPtr(new Amount(*o->getFounds()[amount->getCurrency()] - individualDebit)));
        }
    } else {
        throw AccountManagerMethodException(TOO_EXPENSIVE);
    }
}

void AccountManager::addOwnerToAccount(const AccountSPtr &account, const ClientSPtr &owner) {
    if (account == nullptr) {
        throw AccountManagerMethodException(NULL_ACCOUNT);
    }
    if (owner == nullptr) {
        throw AccountManagerMethodException(NULL_OWNER);
    }
    if (!account->addOwner(owner)) {
        throw AccountManagerMethodException(CLIENT_EXISTS);
    }
    if (!owner->addAccount(account)) {
        throw AccountManagerMethodException(ACCOUNT_EXISTS);
    }
}

void AccountManager::removeOwnerFromAccount(const AccountSPtr &account, const ClientSPtr &owner) {
    if (account == nullptr) {
        throw AccountManagerMethodException(NULL_ACCOUNT);
    }
    if (owner == nullptr) {
        throw AccountManagerMethodException(NULL_OWNER);
    }
    if (!account->removeOwner(owner)) {
        throw AccountManagerMethodException(NO_CLIENT_IN_ACCOUNT);
    }
    if (!owner->removeAccount(account)) {
        throw AccountManagerMethodException(NO_ACCOUNT_IN_CLIENT);
    }
}

string AccountManager::accountInfo(const AccountSPtr &account) const {
    if (account == nullptr) {
        throw AccountManagerMethodException(NULL_ACCOUNT);
    }
    return account->toString();
}

AccountRepositorySPtr AccountManager::getRepository(AccountRepositoriesEnum repository) const {
    AccountRepositorySPtr repo;
    switch (repository) {
        case SavingsAccountsRepo:
            repo = savingsAccounts;
            break;
        case CurrencyAccountsRepo:
            repo = currencyAccounts;
            break;
        case JointAccountsRepo:
            repo = jointAccounts;
            break;
        case ClosedAccountsRepo:
            repo = closedAccounts;
            break;
    }
    return repo;
}
