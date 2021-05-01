//
// Created by student on 01.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "model/ClientType.h"
#include "model/NewClient.h"
#include "model/NormalClient.h"
#include "model/TrustworthyClient.h"
#include "model/CommonDebtor.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectClientType)

    BOOST_AUTO_TEST_CASE(ClientTypeFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientTypeFunctionalityTest:");

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(nullptr);
        BOOST_CHECK_THROW(NewClient(1.0f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(1.0f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(1.0f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(1.0f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(-600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(1.0f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(0.01f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(1.1f, 5, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_THROW(NewClient(1.0f, -1, move(maxAmounts)), ClientTypeConstructionException);

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        BOOST_CHECK_NO_THROW(NewClient(1.0f, 1, move(maxAmounts)));

        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));
        NewClientUPtr Wojciechowski(new NewClient(1.0f, 5, move(maxAmounts)));

        BOOST_CHECK_THROW(Wojciechowski->setBaseServiceTier(-1), ClientTypeModificationException);
        BOOST_CHECK_NO_THROW(Wojciechowski->setBaseServiceTier(5));
        BOOST_CHECK_EQUAL(Wojciechowski->getBaseServiceTier(), 5);

        BOOST_CHECK_THROW(Wojciechowski->setServiceCostMultiplier(0.01f), ClientTypeModificationException);
        BOOST_CHECK_THROW(Wojciechowski->setServiceCostMultiplier(2.0f), ClientTypeModificationException);
        BOOST_CHECK_NO_THROW(Wojciechowski->setServiceCostMultiplier(0.9f));
        BOOST_CHECK_CLOSE(Wojciechowski->getServiceCostMultiplier(), 0.9f, 0.1f);

        BOOST_CHECK_THROW(Wojciechowski->setMaximumAmount(nullptr), ClientTypeModificationException);
        BOOST_CHECK_THROW(Wojciechowski->setMaximumAmount(AmountUPtr(new Amount(0L, -1, PLN))), ClientTypeModificationException);

        BOOST_TEST_MESSAGE(Wojciechowski->toString());

        BOOST_TEST_MESSAGE("End of ClientTypeFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(CommonDebtorFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CommonDebtorFunctionalityTest:");

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));

        CommonDebtorUPtr Wojciechowski(new CommonDebtor(1.0f, 5, move(maxAmounts)));

        BOOST_CHECK_THROW(Wojciechowski->calculateServiceCostCoefficient(50, nullptr), ClientTypeMethodException);
        BOOST_CHECK_THROW(Wojciechowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(-5L, 0, PLN))), ClientTypeMethodException);
        BOOST_CHECK_CLOSE(Wojciechowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5L, 0, PLN))), 0.50f, 0.01f);
        BOOST_CHECK_CLOSE(Wojciechowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(500L, 0, PLN))), 0.45f, 0.01f);
        BOOST_CHECK_CLOSE(Wojciechowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5000L, 0, PLN))), 0.40f, 0.01f);
        BOOST_CHECK(Wojciechowski->calculateServiceCostCoefficient(100, AmountSPtr(new Amount(2000L, 0, PLN))) >= 0.1f);
        BOOST_CHECK(Wojciechowski->calculateServiceCostCoefficient(-200, AmountSPtr(new Amount(5L, 0, PLN))) <= 2.0f);

        BOOST_CHECK_EQUAL(Wojciechowski->calculateServiceLevel(50), 55);
        BOOST_CHECK(Wojciechowski->calculateServiceLevel(-30) >= 0);
        BOOST_CHECK(Wojciechowski->calculateServiceLevel(100) <= 100);

        BOOST_CHECK_EQUAL(Wojciechowski->briefClientTypeInfo(), "Common Debtor");

        BOOST_TEST_MESSAGE("End of CommonDebtorFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(NewClientFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of NewClientFunctionalityTest:");

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(10000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));

        NewClientUPtr Nowak(new NewClient(0.75f, 10, move(maxAmounts)));

        BOOST_CHECK_THROW(Nowak->calculateServiceCostCoefficient(50, nullptr), ClientTypeMethodException);
        BOOST_CHECK_THROW(Nowak->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(-5L, 0, PLN))), ClientTypeMethodException);
        BOOST_CHECK_CLOSE(Nowak->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5L, 0, PLN))), 0.50f, 0.01f);
        BOOST_CHECK_CLOSE(Nowak->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(50L, 0, PLN))), 0.45f, 0.01f);
        BOOST_CHECK_CLOSE(Nowak->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(500L, 0, PLN))), 0.40f, 0.01f);
        BOOST_CHECK_CLOSE(Nowak->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5000L, 0, PLN))), 0.35f, 0.01f);
        BOOST_CHECK(Nowak->calculateServiceCostCoefficient(100, AmountSPtr(new Amount(2000L, 0, PLN))) >= 0.1f);
        BOOST_CHECK(Nowak->calculateServiceCostCoefficient(-200, AmountSPtr(new Amount(5L, 0, PLN))) <= 2.0f);

        BOOST_CHECK_EQUAL(Nowak->calculateServiceLevel(50), 60);
        BOOST_CHECK(Nowak->calculateServiceLevel(-20) >= 0);
        BOOST_CHECK(Nowak->calculateServiceLevel(100) <= 100);

        BOOST_CHECK_EQUAL(Nowak->briefClientTypeInfo(), "New Client");

        BOOST_TEST_MESSAGE("End of NewClientFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(NormalClientFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of NormalClientFunctionalityTest:");

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(100000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));

        NormalClientUPtr Kowalski(new NormalClient(0.5f, 15, move(maxAmounts)));

        BOOST_CHECK_THROW(Kowalski->calculateServiceCostCoefficient(50, nullptr), ClientTypeMethodException);
        BOOST_CHECK_THROW(Kowalski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(-5L, 0, PLN))), ClientTypeMethodException);
        BOOST_CHECK_CLOSE(Kowalski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5L, 0, PLN))), 0.50f, 0.01f);
        BOOST_CHECK_CLOSE(Kowalski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(50L, 0, PLN))), 0.35f, 0.01f);
        BOOST_CHECK_CLOSE(Kowalski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(500L, 0, PLN))), 0.30f, 0.01f);
        BOOST_CHECK_CLOSE(Kowalski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5000L, 0, PLN))), 0.25f, 0.01f);
        BOOST_CHECK(Kowalski->calculateServiceCostCoefficient(100, AmountSPtr(new Amount(2000L, 0, PLN))) >= 0.1f);
        BOOST_CHECK(Kowalski->calculateServiceCostCoefficient(-200, AmountSPtr(new Amount(5L, 0, PLN))) <= 2.0f);

        BOOST_CHECK_EQUAL(Kowalski->calculateServiceLevel(50), 70);
        BOOST_CHECK(Kowalski->calculateServiceLevel(-20) >= 0);
        BOOST_CHECK(Kowalski->calculateServiceLevel(100) <= 100);

        BOOST_CHECK_EQUAL(Kowalski->briefClientTypeInfo(), "Normal Client");

        BOOST_TEST_MESSAGE("End of NormalClientFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(TrustworthyClientFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of TrustworthyClientFunctionalityTest:");

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(100000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));

        TrustworthyClientUPtr Kwiatkowski(new TrustworthyClient(0.25f, 25, move(maxAmounts)));

        BOOST_CHECK_THROW(Kwiatkowski->calculateServiceCostCoefficient(50, nullptr), ClientTypeMethodException);
        BOOST_CHECK_THROW(Kwiatkowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(-5L, 0, PLN))), ClientTypeMethodException);
        BOOST_CHECK_CLOSE(Kwiatkowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5L, 0, PLN))), 0.50f, 0.01f);
        BOOST_CHECK_CLOSE(Kwiatkowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(50L, 0, PLN))), 0.25f, 0.01f);
        BOOST_CHECK_CLOSE(Kwiatkowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(500L, 0, PLN))), 0.20f, 0.01f);
        BOOST_CHECK_CLOSE(Kwiatkowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5000L, 0, PLN))), 0.15f, 0.01f);
        BOOST_CHECK_CLOSE(Kwiatkowski->calculateServiceCostCoefficient(50, AmountSPtr(new Amount(5000L, 0, PLN))), 0.15f, 0.01f);
        BOOST_CHECK(Kwiatkowski->calculateServiceCostCoefficient(100, AmountSPtr(new Amount(2000L, 0, PLN))) >= 0.1f);
        BOOST_CHECK(Kwiatkowski->calculateServiceCostCoefficient(-200, AmountSPtr(new Amount(5L, 0, PLN))) <= 2.0f);

        BOOST_CHECK_EQUAL(Kwiatkowski->calculateServiceLevel(50), 85);
        BOOST_CHECK(Kwiatkowski->calculateServiceLevel(-30) >= 0);
        BOOST_CHECK(Kwiatkowski->calculateServiceLevel(100) <= 100);

        BOOST_CHECK_EQUAL(Kwiatkowski->briefClientTypeInfo(), "Trustworthy Client");

        BOOST_TEST_MESSAGE("End of TrustworthyClientFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()