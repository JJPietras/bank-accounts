//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_NEWCLIENT_H
#define BANKKONTA_NEWCLIENT_H


#include "ClientType.h"

class NewClient : public ClientType {
public:
    NewClient(float serviceCostMultiplier, short baseServiceTier, std::vector<std::unique_ptr<Amount>> maximumAmount);

    ~NewClient() override;

    float calculateServiceCostCoefficient(short creditworthiness, std::shared_ptr<Amount> serviceCost) override;

    short calculateServiceLevel(short creditworthiness) override;

    std::string briefClientTypeInfo() override;

};

typedef std::shared_ptr<NewClient> NewClientSPtr;
typedef std::unique_ptr<NewClient> NewClientUPtr;

#endif //BANKKONTA_NEWCLIENT_H
