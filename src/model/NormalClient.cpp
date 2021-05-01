//
// Created by student on 21.12.2019.
//

#include <boost/algorithm/clamp.hpp>

#include "model/NormalClient.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

NormalClient::NormalClient(float serviceCostMultiplier, short baseServiceTier, vector<AmountUPtr> maximumAmount) : ClientType(serviceCostMultiplier, baseServiceTier, move(maximumAmount)) {}

NormalClient::~NormalClient() = default;

float NormalClient::calculateServiceCostCoefficient(short creditworthiness, AmountSPtr serviceCost) {
    if (serviceCost == nullptr) {
        throw ClientTypeMethodException(NULL_AMOUNT);
    }
    if (*serviceCost < Amount(0L, 0, serviceCost->getCurrency())) {
        throw ClientTypeMethodException(NEGATIVE_AMOUNT);
    }
    float out = 1.0f;

    if (serviceCost->getMainUnit() > 1000L) {
        out *= 0.75f;
    } else if (serviceCost->getMainUnit() > 100L) {
        out *= 0.80f;
    } else if (serviceCost->getMainUnit() > 10L) {
        out *= 0.85f;
    }

    out -= (float) creditworthiness / 100.00f;
    return boost::algorithm::clamp(out, 0.1f, 2.0f);
}

short NormalClient::calculateServiceLevel(short creditworthiness) {
    return boost::algorithm::clamp((float) getBaseServiceTier() + 1.1f * (float) creditworthiness, 0, 100);
}

string NormalClient::briefClientTypeInfo() {
    return "Normal Client";
}
