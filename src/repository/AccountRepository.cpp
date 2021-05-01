//
// Created by student on 21.12.2019.
//

#include <algorithm>

#include "repository/AccountRepository.h"
#include "model/Account.h"
#include "exceptions/AccountExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

AccountRepository::AccountRepository() = default;

AccountRepository::~AccountRepository() = default;

void AccountRepository::add(const AccountSPtr &element) {
    if (element == nullptr) {
        throw AccountRepositoryException(NULL_ACCOUNT);
    }
    if (accounts.empty()) {
        accounts.emplace_back(element);
        return;
    }
    auto possibleExistingAccount = find([&element](const AccountSPtr &account) { return element->getUuid() == account->getUuid(); });
    if (possibleExistingAccount == nullptr) {
        accounts.emplace_back(element);
    } else {
        throw AccountRepositoryException(ACCOUNT_EXISTS);
    }
}

void AccountRepository::remove(const AccountSPtr &element) {
    if (element == nullptr) {
        throw AccountRepositoryException(NULL_ACCOUNT);
    }
    if (accounts.empty()) {
        throw AccountRepositoryException(EMPTY_COLLECTION);
    }

    auto accountIterator = accounts.begin();
    for (; accountIterator != accounts.end(); accountIterator++) {
        if ((*accountIterator)->getUuid() == element->getUuid()) {
            accounts.erase(accountIterator);
            return;
        }
    }
    throw AccountRepositoryException(ACCOUNT_EXISTS);
}

vector<AccountSPtr> AccountRepository::getAll() {
    return accounts;
}

AccountSPtr AccountRepository::find(const function<bool(AccountSPtr account)> &predicate) {
    auto found = find_if(accounts.begin(), accounts.end(), predicate);
    if (found == accounts.end()) {
        return nullptr;
    }
    return *found;
}
