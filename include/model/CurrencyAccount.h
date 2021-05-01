//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CURRENCYACCOUNT_H
#define BANKKONTA_CURRENCYACCOUNT_H


#include "Account.h"

class CurrencyAccount : public Account {
public:
    explicit CurrencyAccount(std::unique_ptr<Amount> amount);

    CurrencyAccount(std::unique_ptr<boost::local_time::local_date_time> openingTime, std::unique_ptr<Amount> amount);

    ~CurrencyAccount() override;

    bool addOwner(const std::shared_ptr<Client> &owner) override;

    std::shared_ptr<Amount> calculateTransferCost() const override;

    std::string briefAccountInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<CurrencyAccount> CurrencyAccountSPtr;
typedef std::unique_ptr<CurrencyAccount> CurrencyAccountUPtr;

#endif //BANKKONTA_CURRENCYACCOUNT_H
