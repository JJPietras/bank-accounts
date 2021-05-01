//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_JOINTACCOUNT_H
#define BANKKONTA_JOINTACCOUNT_H


#include "Account.h"

class Amount;

class JointAccount : public Account {
private:
    short maxOwners;

public:
    JointAccount(std::unique_ptr<Amount> amount, short maxOwners);

    JointAccount(std::unique_ptr<boost::local_time::local_date_time> openingTime, std::unique_ptr<Amount> amount, short maxOwners);

    ~JointAccount() override;

    bool addOwner(const std::shared_ptr<Client> &owner) override;

    std::shared_ptr<Amount> calculateTransferCost() const override;

    std::string briefAccountInfo() const override;

    std::string toString() const override;

};

typedef std::shared_ptr<JointAccount> JointAccountSPtr;
typedef std::unique_ptr<JointAccount> JointAccountUPtr;

#endif //BANKKONTA_JOINTACCOUNT_H
