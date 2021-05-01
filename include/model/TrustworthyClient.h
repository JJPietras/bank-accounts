//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_TRUSTWORTHYCLIENT_H
#define BANKKONTA_TRUSTWORTHYCLIENT_H


#include "ClientType.h"

class TrustworthyClient : public ClientType {
public:
    TrustworthyClient(float serviceCostMultiplier, short baseServiceTier, std::vector<std::unique_ptr<Amount>> maximumAmount);

    ~TrustworthyClient() override;

    float calculateServiceCostCoefficient(short creditworthiness, std::shared_ptr<Amount> serviceCost) override;

    short calculateServiceLevel(short creditworthiness) override;

    std::string briefClientTypeInfo() override;

};

typedef std::shared_ptr<TrustworthyClient> TrustworthyClientSPtr;
typedef std::unique_ptr<TrustworthyClient> TrustworthyClientUPtr;

#endif //BANKKONTA_TRUSTWORTHYCLIENT_H
