//
// Created by student on 02.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "model/CurrencyAccount.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/Address.h"
#include "model/JointAccount.h"
#include "model/DebitCard.h"
#include "model/SavingsAccount.h"
#include "exceptions/AccountExceptions.h"


using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectAccount)

    ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
    ClientSPtr client2(new Client("Tom", "Kowalski", "12312323", AddressUPtr(new Address("Piotrkowska", "4"))));
    ClientSPtr client3(new Client("Rob", "Kowalski", "90312323", AddressUPtr(new Address("Piotrkowska", "19"))));

    BOOST_AUTO_TEST_CASE(AccountFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of AccountFunctionalityTest:");

        BOOST_CHECK_THROW(JointAccountUPtr(new JointAccount(AmountUPtr(new Amount(200L, 30, USD)), 2)), JointAccountConstructionException);
        BOOST_CHECK_THROW(JointAccountUPtr(new JointAccount(nullptr, 2)), AccountConstructionException);

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        unique_ptr<boost::local_time::local_date_time> ldt1(new boost::local_time::local_date_time(time, zone));
        unique_ptr<boost::local_time::local_date_time> ldt2(new boost::local_time::local_date_time(time, zone));
        unique_ptr<boost::local_time::local_date_time> ldt3(new boost::local_time::local_date_time(time, zone));

        BOOST_CHECK_THROW(JointAccountUPtr(new JointAccount(move(ldt1), AmountUPtr(new Amount(200L, 30, USD)), 2)), JointAccountConstructionException);
        BOOST_CHECK_THROW(JointAccountUPtr(new JointAccount(move(ldt2), nullptr, 2)), AccountConstructionException);
        BOOST_CHECK_THROW(JointAccountUPtr(new JointAccount(nullptr, AmountUPtr(new Amount(200L, 30, PLN)), 2)), AccountConstructionException);

        JointAccountSPtr AC1(new JointAccount(AmountUPtr(new Amount(200L, 30, PLN)), 2));

        BOOST_CHECK_EQUAL(AC1->getOwners().size(), 0UL);
        BOOST_CHECK_EQUAL(AC1->getCards().size(), 0UL);
        BOOST_CHECK_EQUAL(*AC1->getAmount(), Amount(200L, 30, PLN));
        BOOST_CHECK(AC1->getUuid() != nullptr);

        BOOST_CHECK_THROW(AC1->setAmount(AmountUPtr(new Amount(30L, 10, USD))), AccountModificationException);
        BOOST_CHECK_EQUAL(*AC1->getOpeningTime(), *ldt3);
        BOOST_CHECK(AC1->getClosureTime() == nullptr);

        DebitCardSPtr card1(new DebitCard(client1, AC1));

        BOOST_CHECK(!AC1->removeCard(card1));
        BOOST_CHECK(AC1->addCard(card1));
        BOOST_CHECK(AC1->getCards().size() == 1);
        BOOST_CHECK(AC1->removeCard(card1));
        BOOST_CHECK(AC1->getCards().empty());

        BOOST_CHECK_THROW(AC1->closeAccount(), AccountMethodException);
        BOOST_CHECK(AC1->addOwner(client1));
        BOOST_CHECK_EQUAL(*AC1->closeAccount(), Amount(200L, 30, PLN));
        BOOST_CHECK(AC1->getClosureTime() != nullptr);
        BOOST_CHECK_THROW(AC1->closeAccount(), AccountMethodException);

        BOOST_TEST_MESSAGE("End of AccountFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(SavingsAccountFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of SavingsAccountFunctionalityTest:");

        SavingsAccountSPtr AC2(new SavingsAccount(AmountUPtr(new Amount(200L, 30, PLN))));

        BOOST_CHECK_EQUAL(*AC2->calculateTransferCost(), Amount(5L, 0, PLN));

        BOOST_CHECK_NO_THROW(AC2->setAmount(AmountUPtr(new Amount(2000L, 80, PLN))));
        BOOST_CHECK_EQUAL(*AC2->calculateTransferCost(), Amount(2L, 0, PLN));

        BOOST_CHECK_NO_THROW(AC2->setAmount(AmountUPtr(new Amount(20000L, 80, PLN))));
        BOOST_CHECK_EQUAL(*AC2->calculateTransferCost(), Amount(1L, 50, PLN));

        BOOST_CHECK_NO_THROW(AC2->setAmount(AmountUPtr(new Amount(70000L, 80, PLN))));
        BOOST_CHECK_EQUAL(*AC2->calculateTransferCost(), Amount(1L, 0, PLN));

        BOOST_CHECK_NO_THROW(AC2->setAmount(AmountUPtr(new Amount(200000L, 80, PLN))));
        BOOST_CHECK_EQUAL(*AC2->calculateTransferCost(), Amount(0L, 0, PLN));

        BOOST_TEST_MESSAGE(AC2->briefAccountInfo());
        BOOST_TEST_MESSAGE(AC2->toString());

        BOOST_TEST_MESSAGE("End of SavingsAccountFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(JointAccountFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of JointAccountFunctionalityTest:");

        JointAccountSPtr AC3(new JointAccount(AmountUPtr(new Amount(200L, 30, PLN)), 2));
        JointAccountSPtr AC4(new JointAccount(AmountUPtr(new Amount(200L, 30, PLN)), 3));

        BOOST_CHECK_EQUAL(*AC3->calculateTransferCost(), Amount(4L, 0, PLN));

        BOOST_CHECK(!AC3->removeOwner(client1));
        BOOST_CHECK(AC3->addOwner(client1));
        BOOST_CHECK(AC3->getOwners().size() == 1);
        BOOST_CHECK(AC3->removeOwner(client1));
        BOOST_CHECK(AC3->getOwners().empty());
        BOOST_CHECK(AC3->addOwner(client1));
        BOOST_CHECK(AC3->addOwner(client2));
        BOOST_CHECK(!AC3->addOwner(client3));
        BOOST_CHECK(!AC3->addOwner(client2));

        BOOST_CHECK_EQUAL(*AC3->calculateTransferCost(), Amount(2L, 0, PLN));

        BOOST_CHECK(AC4->addOwner(client1));
        BOOST_CHECK(AC4->addOwner(client2));
        BOOST_CHECK(AC4->addOwner(client3));

        BOOST_CHECK_EQUAL(*AC4->calculateTransferCost(), Amount(1L, 34, PLN));

        BOOST_TEST_MESSAGE(AC4->briefAccountInfo());
        BOOST_TEST_MESSAGE(AC4->toString());

        BOOST_TEST_MESSAGE("End of JointAccountFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(CurrencyAccountFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CurrencyAccountFunctionalityTest:");

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        unique_ptr<boost::local_time::local_date_time> ldt4(new boost::local_time::local_date_time(time, zone));

        CurrencyAccountSPtr AC5(new CurrencyAccount(AmountUPtr(new Amount(200L, 30, USD))));
        CurrencyAccountSPtr AC6(new CurrencyAccount(move(ldt4), AmountUPtr(new Amount(200L, 30, USD))));

        BOOST_CHECK_THROW(AC5->setAmount(AmountUPtr(new Amount(10L, 30, GBP))), AccountModificationException);
        BOOST_CHECK_NO_THROW(AC5->setAmount(AmountUPtr(new Amount(10L, 80, USD))));
        BOOST_CHECK_EQUAL(*AC5->getAmount(), Amount(10L, 80, USD));

        BOOST_CHECK_EQUAL(*AC5->calculateTransferCost(), Amount(5L, 75, USD));

        BOOST_CHECK_NO_THROW(AC5->setAmount(AmountUPtr(new Amount(150L, 80, USD))));

        BOOST_CHECK_EQUAL(*AC5->calculateTransferCost(), Amount(3L, 25, USD));

        BOOST_CHECK(AC5->addOwner(client1));
        BOOST_CHECK(!AC5->addOwner(client2));

        BOOST_TEST_MESSAGE(AC5->briefAccountInfo());
        BOOST_TEST_MESSAGE(AC5->toString());

        BOOST_TEST_MESSAGE("End of CurrencyAccountFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()