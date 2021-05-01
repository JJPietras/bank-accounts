//
// Created by student on 21.12.2019.
//

#include <boost/algorithm/clamp.hpp>

#include "model/NewClient.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

NewClient::NewClient(float serviceCostMultiplier, short baseServiceTier, vector<AmountUPtr> maximumAmount) : ClientType(serviceCostMultiplier, baseServiceTier, move(maximumAmount)) {}

NewClient::~NewClient() = default;

float NewClient::calculateServiceCostCoefficient(short creditworthiness, AmountSPtr serviceCost) {
    if (serviceCost == nullptr) {
        throw ClientTypeMethodException(NULL_AMOUNT);
    }
    if (*serviceCost < Amount(0L, 0, serviceCost->getCurrency())) {
        throw ClientTypeMethodException(NEGATIVE_AMOUNT);
    }
    float out = 1.0f;

    if (serviceCost->getMainUnit() > 1000L) {
        out *= 0.85f;
    } else if (serviceCost->getMainUnit() > 100L) {
        out *= 0.90f;
    } else if (serviceCost->getMainUnit() > 10L) {
        out *= 0.95f;
    }

    out -= (float) creditworthiness / 100.00f;
    return boost::algorithm::clamp(out, 0.1f, 2.0f);
}

short NewClient::calculateServiceLevel(short creditworthiness) {
    return boost::algorithm::clamp(getBaseServiceTier() + creditworthiness, 0, 100);
}

string NewClient::briefClientTypeInfo() {
    return "New Client";
}
