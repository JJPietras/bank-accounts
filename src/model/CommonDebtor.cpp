//
// Created by student on 21.12.2019.
//

#include <boost/algorithm/clamp.hpp>

#include "model/CommonDebtor.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

CommonDebtor::CommonDebtor(float serviceCostMultiplier, short baseServiceTier, vector<AmountUPtr> maximumAmount) : ClientType(serviceCostMultiplier, baseServiceTier, move(maximumAmount)) {}

CommonDebtor::~CommonDebtor() = default;

float CommonDebtor::calculateServiceCostCoefficient(short creditworthiness, AmountSPtr serviceCost) {
    if (serviceCost == nullptr) {
        throw ClientTypeMethodException(NULL_AMOUNT);
    }
    if (*serviceCost < Amount(0L, 0, serviceCost->getCurrency())) {
        throw ClientTypeMethodException(NEGATIVE_AMOUNT);
    }
    float out = 1.0f;

    if (serviceCost->getMainUnit() > 1000L) {
        out *= 0.90f;
    } else if (serviceCost->getMainUnit() > 100L) {
        out *= 0.95f;
    }

    out -= (float) creditworthiness / 100.00f;
    return boost::algorithm::clamp(out, 0.1f, 2.0f);
}

short CommonDebtor::calculateServiceLevel(short creditworthiness) {
    return boost::algorithm::clamp(getBaseServiceTier() + creditworthiness, 0, 100);
}

string CommonDebtor::briefClientTypeInfo() {
    return "Common Debtor";
}
