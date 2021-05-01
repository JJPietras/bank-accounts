//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CLIENTTYPE_H
#define BANKKONTA_CLIENTTYPE_H


#include <memory>
#include <vector>

class Amount;

class ClientType {
private:
    float serviceCostMultiplier;
    short baseServiceTier;
    std::vector<std::unique_ptr<Amount>> maximumAmount;

public:
    ClientType(float serviceCostMultiplier, short baseServiceTier, std::vector<std::unique_ptr<Amount>> maximumAmount);

    virtual ~ClientType();

    float getServiceCostMultiplier() const;

    void setServiceCostMultiplier(float _serviceCostMultiplier);

    short getBaseServiceTier() const;

    void setBaseServiceTier(short _baseServiceTier);

    const std::vector<std::unique_ptr<Amount>> &getMaximumAmount() const;

    void setMaximumAmount(std::unique_ptr<Amount> _maximumAmount);

    virtual float calculateServiceCostCoefficient(short creditworthiness, std::shared_ptr<Amount> serviceCost) = 0;

    virtual short calculateServiceLevel(short creditworthiness) = 0;

    virtual std::string briefClientTypeInfo() = 0;

    std::string toString();

};

typedef std::shared_ptr<ClientType> ClientTypeSPtr;
typedef std::unique_ptr<ClientType> ClientTypeUPtr;

#endif //BANKKONTA_CLIENTTYPE_H
