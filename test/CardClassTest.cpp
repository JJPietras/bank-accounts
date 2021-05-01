//
// Created by student on 03.01.2020.
//

#include <boost/test/unit_test.hpp>

#include "model/CardClass.h"
#include "model/StandardCard.h"
#include "model/SilverCard.h"
#include "model/GoldCard.h"
#include "model/PlatinumCard.h"
#include "model/TitaniumCard.h"
#include "model/JointAccount.h"
#include "model/Amount.h"
#include "model/Address.h"
#include "model/Client.h"
#include "model/NormalClient.h"
#include "exceptions/CardExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectCardClass)

    ClientSPtr Jan(new Client("Jan", "Kowalski", "2525233", AddressUPtr(new Address("Piotrkowska", "1"))));
    ClientSPtr Krzysztof(new Client("Krzysztof", "Nowak", "7463883", AddressUPtr(new Address("Woronicza", "17"))));

    BOOST_AUTO_TEST_CASE(CardClassFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CardClassFunctionalityTest:");

        BOOST_CHECK_THROW(StandardCard(nullptr, AmountUPtr(new Amount(5L, 0, PLN))), CardClassConstructionException);
        BOOST_CHECK_THROW(StandardCard(AmountUPtr(new Amount(5L, 0, PLN)), nullptr), CardClassConstructionException);
        BOOST_CHECK_THROW(StandardCard(AmountUPtr(new Amount(-5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))), CardClassConstructionException);
        BOOST_CHECK_THROW(StandardCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(-5L, 0, PLN))), CardClassConstructionException);

        StandardCardSPtr StC(new StandardCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));

        BOOST_TEST_MESSAGE(StC->toString());

        BOOST_TEST_MESSAGE("End of CardClassFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(StandardCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of StandardCardFunctionalityTest:");

        StandardCardSPtr StC(new StandardCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));
        AccountSPtr account(new JointAccount(AmountUPtr(new Amount(100L, 0, PLN)), 2));

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(100000L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));

        Jan->setClientType(ClientTypeSPtr(new NormalClient(0.5f, 15, move(maxAmounts))));
        Jan->setCreditworthiness(50);

        vector<AmountUPtr> maxAmounts2;
        maxAmounts2.emplace_back(AmountUPtr(new Amount(100000L, 0, PLN)));
        maxAmounts2.emplace_back(AmountUPtr(new Amount(500L, 0, EUR)));
        maxAmounts2.emplace_back(AmountUPtr(new Amount(400L, 0, GBP)));
        maxAmounts2.emplace_back(AmountUPtr(new Amount(600L, 0, USD)));

        Krzysztof->setClientType(ClientTypeSPtr(new NormalClient(0.4f, 10, move(maxAmounts2))));
        Krzysztof->setCreditworthiness(30);

        BOOST_CHECK_THROW(StC->calculateWithdrawCost(nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(StC->payIntoAccount(nullptr, account), CardClassMethodException);
        BOOST_CHECK_THROW(StC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(StC->calculateWithdrawCost(account), logic_error);

        account->addOwner(Jan);
        account->addOwner(Krzysztof);

        BOOST_CHECK_EQUAL(*StC->calculateWithdrawCost(account), Amount(3L, 0, PLN));

        BOOST_CHECK(StC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), account));
        BOOST_CHECK_EQUAL(*account->getAmount(), Amount(195L, 0, PLN));

        BOOST_CHECK_EQUAL(StC->briefCardClassInfo(), "Standard Card");

        BOOST_TEST_MESSAGE("End of StandardCardFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(SilverCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of SilverCardFunctionalityTest:");

        SilverCardSPtr SvC(new SilverCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));
        AccountSPtr account(new JointAccount(AmountUPtr(new Amount(100L, 0, PLN)), 2));

        BOOST_CHECK_THROW(SvC->calculateWithdrawCost(nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(SvC->payIntoAccount(nullptr, account), CardClassMethodException);
        BOOST_CHECK_THROW(SvC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(SvC->calculateWithdrawCost(account), CardClassMethodException);

        account->addOwner(Jan);
        account->addOwner(Krzysztof);

        BOOST_CHECK_EQUAL(*SvC->calculateWithdrawCost(account), Amount(2L, 40, PLN));

        BOOST_CHECK(SvC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), account));
        BOOST_CHECK_EQUAL(*account->getAmount(), Amount(195L, 0, PLN));

        BOOST_CHECK_EQUAL(SvC->briefCardClassInfo(), "Silver Card");

        BOOST_TEST_MESSAGE("End of SilverCardFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(GoldCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of GoldCardFunctionalityTest:");

        GoldCardSPtr GdC(new GoldCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));
        AccountSPtr account(new JointAccount(AmountUPtr(new Amount(100L, 0, PLN)), 2));

        BOOST_CHECK_THROW(GdC->calculateWithdrawCost(nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(GdC->payIntoAccount(nullptr, account), CardClassMethodException);
        BOOST_CHECK_THROW(GdC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(GdC->calculateWithdrawCost(account), CardClassMethodException);

        account->addOwner(Jan);
        account->addOwner(Krzysztof);

        BOOST_CHECK_EQUAL(*GdC->calculateWithdrawCost(account), Amount(1L, 80, PLN));

        BOOST_CHECK(GdC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), account));
        BOOST_CHECK_EQUAL(*account->getAmount(), Amount(195L, 0, PLN));

        BOOST_CHECK_EQUAL(GdC->briefCardClassInfo(), "Gold Card");

        BOOST_TEST_MESSAGE("End of GoldCardFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(PlatinumCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of PlatinumCardFunctionalityTest:");

        PlatinumCardSPtr PtC(new PlatinumCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));
        AccountSPtr account(new JointAccount(AmountUPtr(new Amount(100L, 0, PLN)), 2));

        BOOST_CHECK_THROW(PtC->calculateWithdrawCost(nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(PtC->payIntoAccount(nullptr, account), CardClassMethodException);
        BOOST_CHECK_THROW(PtC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(PtC->calculateWithdrawCost(account), CardClassMethodException);

        account->addOwner(Jan);
        account->addOwner(Krzysztof);

        BOOST_CHECK_EQUAL(*PtC->calculateWithdrawCost(account), Amount(1L, 20, PLN));

        BOOST_CHECK(PtC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), account));
        BOOST_CHECK_EQUAL(*account->getAmount(), Amount(195L, 0, PLN));

        BOOST_CHECK_EQUAL(PtC->briefCardClassInfo(), "Platinum Card");

        BOOST_TEST_MESSAGE("End of PlatinumCardFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(TitaniumCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of TitaniumCardFunctionalityTest:");

        TitaniumCardSPtr TtC(new TitaniumCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));
        AccountSPtr account(new JointAccount(AmountUPtr(new Amount(100L, 0, PLN)), 2));

        BOOST_CHECK_THROW(TtC->calculateWithdrawCost(nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(TtC->payIntoAccount(nullptr, account), CardClassMethodException);
        BOOST_CHECK_THROW(TtC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), nullptr), CardClassMethodException);
        BOOST_CHECK_THROW(TtC->calculateWithdrawCost(account), CardClassMethodException);

        account->addOwner(Jan);
        account->addOwner(Krzysztof);

        BOOST_CHECK_EQUAL(*TtC->calculateWithdrawCost(account), Amount(0L, 60, PLN));

        BOOST_CHECK(TtC->payIntoAccount(AmountUPtr(new Amount(100L, 0, PLN)), account));
        BOOST_CHECK_EQUAL(*account->getAmount(), Amount(195L, 0, PLN));

        BOOST_CHECK_EQUAL(TtC->briefCardClassInfo(), "Titanium Card");

        BOOST_TEST_MESSAGE("End of TitaniumCardFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()