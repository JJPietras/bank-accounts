//
// Created by student on 21.12.2019.
//

#define CURRENCIES 4

#include <sstream>
#include <utility>

#include "model/ClientType.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

ClientType::ClientType(float serviceCostMultiplier, short baseServiceTier, vector<AmountUPtr> maximumAmount)
        : serviceCostMultiplier(serviceCostMultiplier), baseServiceTier(baseServiceTier), maximumAmount(move(maximumAmount)) {
    for (int i = 0; i < CURRENCIES; i++) {
        if (this->maximumAmount.size() < CURRENCIES || this->maximumAmount[i] == nullptr) {
            throw ClientTypeConstructionException(NULL_AMOUNT);
        }
        if (this->maximumAmount[i]->getMainUnit() < 0L) {
            throw ClientTypeConstructionException(NEGATIVE_MAX_AMOUNT);
        }
    }
    if (this->serviceCostMultiplier < 0.1f || this->serviceCostMultiplier > 1.0f) {
        throw ClientTypeConstructionException(WRONG_MULTIPLIER);
    }
    if (this->baseServiceTier < 0) {
        throw ClientTypeConstructionException(NEGATIVE_TIER);
    }
}

ClientType::~ClientType() = default;

float ClientType::getServiceCostMultiplier() const {
    return serviceCostMultiplier;
}

void ClientType::setServiceCostMultiplier(float _serviceCostMultiplier) {
    if (_serviceCostMultiplier < 0.1f || _serviceCostMultiplier > 1.0f) {
        throw ClientTypeModificationException(WRONG_MULTIPLIER);
    }
    ClientType::serviceCostMultiplier = _serviceCostMultiplier;
}

short ClientType::getBaseServiceTier() const {
    return baseServiceTier;
}

void ClientType::setBaseServiceTier(short _baseServiceTier) {
    if (_baseServiceTier < 0) {
        throw ClientTypeModificationException(NEGATIVE_TIER);
    }
    baseServiceTier = _baseServiceTier;
}

const vector<AmountUPtr> &ClientType::getMaximumAmount() const {
    return maximumAmount;
}

void ClientType::setMaximumAmount(AmountUPtr _maximumAmount) {
    if (_maximumAmount == nullptr) {
        throw ClientTypeModificationException(NULL_AMOUNT);
    }
    if (*_maximumAmount < Amount(0L, 0, _maximumAmount->getCurrency())) {
        throw ClientTypeModificationException(NEGATIVE_MAX_AMOUNT);
    }
    maximumAmount[_maximumAmount->getCurrency()] = move(_maximumAmount);
}

string ClientType::toString() {
    stringstream stream;
    stream << briefClientTypeInfo() << ", Service Cost Multiplier: " << getServiceCostMultiplier();
    stream << ", Base Service Tier: " << getBaseServiceTier() << ", Maximum Amounts: ";
    stream << *getMaximumAmount()[PLN] << " " << *getMaximumAmount()[EUR] << " " << *getMaximumAmount()[GBP] << " " << *getMaximumAmount()[USD] << " ";
    return stream.str();
}
