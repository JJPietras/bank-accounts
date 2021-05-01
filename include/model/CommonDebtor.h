//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_COMMONDEBTOR_H
#define BANKKONTA_COMMONDEBTOR_H


#include "ClientType.h"

class CommonDebtor : public ClientType {
public:
    CommonDebtor(float serviceCostMultiplier, short baseServiceTier, std::vector<std::unique_ptr<Amount>> maximumAmount);

    ~CommonDebtor() override;

    float calculateServiceCostCoefficient(short creditworthiness, std::shared_ptr<Amount> serviceCost) override;

    short calculateServiceLevel(short creditworthiness) override;

    std::string briefClientTypeInfo() override;

};

typedef std::shared_ptr<CommonDebtor> CommonDebtorSPtr;
typedef std::unique_ptr<CommonDebtor> CommonDebtorUPtr;

#endif //BANKKONTA_COMMONDEBTOR_H
