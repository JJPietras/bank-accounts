//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_SAVINGSACCOUNT_H
#define BANKKONTA_SAVINGSACCOUNT_H


#include "Account.h"

class SavingsAccount : public Account {
public:
    explicit SavingsAccount(std::unique_ptr<Amount> amount);

    SavingsAccount(std::unique_ptr<boost::local_time::local_date_time> openingTime, std::unique_ptr<Amount> amount);

    ~SavingsAccount() override;

    bool addOwner(const std::shared_ptr<Client> &owner) override;

    std::shared_ptr<Amount> calculateTransferCost() const override;

    std::string briefAccountInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<SavingsAccount> SavingsAccountSPtr;
typedef std::unique_ptr<SavingsAccount> SavingsAccountUPtr;

#endif //BANKKONTA_SAVINGSACCOUNT_H
