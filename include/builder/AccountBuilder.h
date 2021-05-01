//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_ACCOUNTBUILDER_H
#define BANKKONTA_ACCOUNTBUILDER_H


#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/date_time/local_time/local_date_time.hpp>

#include "enum/CurrencyType.h"
#include "enum/AccountEnum.h"

class Amount;

class Card;

class Client;

class Account;

class AccountBuilder {
private:
    std::shared_ptr<short> maxOwners = nullptr;
    std::shared_ptr<Amount> amount = nullptr;
    std::shared_ptr<boost::local_time::local_date_time> openingTime = nullptr;
    std::vector<std::shared_ptr<Client>> owners;
    std::shared_ptr<AccountEnum> accountType = nullptr;

public:
    AccountBuilder();

    ~AccountBuilder();

    std::shared_ptr<AccountBuilder> setMaxOwners(const std::shared_ptr<short> &_maxOwners);

    std::shared_ptr<AccountBuilder> setAmount(const std::shared_ptr<Amount> &_amount);

    std::shared_ptr<AccountBuilder> setOpeningTime(const std::shared_ptr<boost::local_time::local_date_time> &_openingTime);

    std::shared_ptr<AccountBuilder> setOwners(const std::vector<std::shared_ptr<Client>> &_owners);

    std::shared_ptr<AccountBuilder> setAccountType(const std::shared_ptr<AccountEnum> &_accountType);

    std::shared_ptr<Account> build();

};

typedef std::shared_ptr<AccountBuilder> AccountBuilderSPtr;
typedef std::unique_ptr<AccountBuilder> AccountBuilderUPtr;

#endif //BANKKONTA_ACCOUNTBUILDER_H
