//
// Created by student on 21.12.2019.
//

#include <boost/algorithm/clamp.hpp>

#include "model/TrustworthyClient.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

TrustworthyClient::TrustworthyClient(float serviceCostMultiplier, short baseServiceTier, vector<AmountUPtr> maximumAmount) : ClientType(serviceCostMultiplier, baseServiceTier, move(maximumAmount)) {}

TrustworthyClient::~TrustworthyClient() = default;

float TrustworthyClient::calculateServiceCostCoefficient(short creditworthiness, AmountSPtr serviceCost) {
    if (serviceCost == nullptr) {
        throw ClientTypeMethodException(NULL_AMOUNT);
    }
    if (*serviceCost < Amount(0L, 0, serviceCost->getCurrency())) {
        throw ClientTypeMethodException(NEGATIVE_AMOUNT);
    }
    float out = 1.0f;

    if (serviceCost->getMainUnit() > 1000L) {
        out *= 0.65f;
    } else if (serviceCost->getMainUnit() > 100L) {
        out *= 0.70f;
    } else if (serviceCost->getMainUnit() > 10L) {
        out *= 0.75f;
    }

    out -= (float) creditworthiness / 100.00f;
    return boost::algorithm::clamp(out, 0.1f, 2.0f);
}

short TrustworthyClient::calculateServiceLevel(short creditworthiness) {
    return boost::algorithm::clamp((float) getBaseServiceTier() + 1.2f * (float) creditworthiness, 0, 100);
}

string TrustworthyClient::briefClientTypeInfo() {
    return "Trustworthy Client";
}
