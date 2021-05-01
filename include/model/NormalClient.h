//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_NORMALCLIENT_H
#define BANKKONTA_NORMALCLIENT_H


#include "ClientType.h"

class NormalClient : public ClientType {
public:
    NormalClient(float serviceCostMultiplier, short baseServiceTier, std::vector<std::unique_ptr<Amount>> maximumAmount);

    ~NormalClient() override;

    float calculateServiceCostCoefficient(short creditworthiness, std::shared_ptr<Amount> serviceCost) override;

    short calculateServiceLevel(short creditworthiness) override;

    std::string briefClientTypeInfo() override;

};

typedef std::shared_ptr<NormalClient> NormalClientSPtr;
typedef std::unique_ptr<NormalClient> NormalClientUPtr;

#endif //BANKKONTA_NORMALCLIENT_H
