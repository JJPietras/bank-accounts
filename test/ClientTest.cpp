//
// Created by student on 02.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "model/Client.h"
#include "model/Address.h"
#include "model/Account.h"
#include "model/SavingsAccount.h"
#include "model/Amount.h"
#include "model/NewClient.h"
#include "model/DebitCard.h"
#include "model/StandardLoan.h"
#include "model/Loan.h"
#include "exceptions/ClientExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectClient)

    ClientSPtr Jan(new Client("Jan", "Kowalski", "2525233", AddressUPtr(new Address("Piotrkowska", "1"))));

    BOOST_AUTO_TEST_CASE(ClientConstructorTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientConstructorTest:");

        BOOST_CHECK_THROW(Client("", "Kowalski", "2525233", AddressUPtr(new Address("Piotrkowska", "1"))), ClientConstructionException);
        BOOST_CHECK_THROW(Client("Jan", "", "2525233", AddressUPtr(new Address("Piotrkowska", "1"))), ClientConstructionException);
        BOOST_CHECK_THROW(Client("Jan", "Kowalski", "", AddressUPtr(new Address("Piotrkowska", "1"))), ClientConstructionException);
        BOOST_CHECK_THROW(Client("Jan", "Kowalski", "2525233", nullptr), ClientConstructionException);

        BOOST_CHECK_EQUAL(Jan->getAddress()->toString(), Jan->getRegisteredAddress()->toString());
        BOOST_CHECK_EQUAL(Jan->getFirstName(), "Jan");
        BOOST_CHECK_EQUAL(Jan->getLastName(), "Kowalski");
        BOOST_CHECK_EQUAL(Jan->getPersonalID(), "2525233");
        BOOST_CHECK(dynamic_cast<NewClient *>(Jan->getClientType().get()) != nullptr);

        BOOST_TEST_MESSAGE("End of ClientConstructorTest");
    }

    BOOST_AUTO_TEST_CASE(ClientCustomSettersTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientCustomSettersTest:");

        BOOST_CHECK_THROW(Jan->setFirstName(""), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setLastName("   "), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setAddress(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setRegisteredAddress(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setClientType(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setFounds(AmountUPtr(nullptr)), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setIncome(AmountUPtr(nullptr)), ClientModificationException);
        BOOST_CHECK_THROW(Jan->setIncome(AmountUPtr(new Amount(-1, 0, PLN))), ClientModificationException);

        BOOST_TEST_MESSAGE("End of ClientCustomSettersTest");
    }

    BOOST_AUTO_TEST_CASE(ClientCollectionsMethodsTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientCollectionsMethodsTest:");

        BOOST_CHECK_EQUAL(Jan->getNumberOfAccounts(), 0);
        BOOST_CHECK_EQUAL(Jan->getNumberOfCards(), 0);
        BOOST_CHECK_EQUAL(Jan->getNumberOfLoans(), 0);

        AmountUPtr amount(new Amount(100, 0, PLN));
        AccountSPtr account(new SavingsAccount(move(amount)));
        BOOST_CHECK(Jan->addAccount(account));
        BOOST_CHECK(!Jan->addAccount(account));
        BOOST_CHECK_EQUAL(Jan->getNumberOfAccounts(), 1);

        CardSPtr card(new DebitCard(Jan, account));
        BOOST_CHECK(Jan->addCard(card));
        BOOST_CHECK(!Jan->addCard(card));
        BOOST_CHECK_EQUAL(Jan->getNumberOfCards(), 1);

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        unique_ptr<boost::local_time::local_date_time> ldt(new boost::local_time::local_date_time(time, zone));
        AmountUPtr secondAmount(new Amount(50, 0, PLN));
        LoanSPtr loan(new StandardLoan(move(ldt), move(secondAmount), Jan));
        BOOST_CHECK(Jan->addLoan(loan));
        BOOST_CHECK(!Jan->addLoan(loan));
        BOOST_CHECK_EQUAL(Jan->getNumberOfLoans(), 1);

        BOOST_CHECK(Jan->removeAccount(account));
        BOOST_CHECK(!Jan->removeAccount(account));
        BOOST_CHECK_EQUAL(Jan->getNumberOfAccounts(), 0);
        BOOST_CHECK(Jan->removeCard(card));
        BOOST_CHECK(!Jan->removeCard(card));
        BOOST_CHECK_EQUAL(Jan->getNumberOfCards(), 0);
        BOOST_CHECK(Jan->removeLoan(loan));
        BOOST_CHECK(!Jan->removeLoan(loan));
        BOOST_CHECK_EQUAL(Jan->getNumberOfLoans(), 0);

        BOOST_CHECK_THROW(Jan->addAccount(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->addLoan(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->addCard(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->removeAccount(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->removeLoan(nullptr), ClientModificationException);
        BOOST_CHECK_THROW(Jan->removeCard(nullptr), ClientModificationException);

        BOOST_TEST_MESSAGE("End of ClientCollectionsMethodsTest");
    }

    BOOST_AUTO_TEST_CASE(ClientStringMethodsTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientStringMethodsTest:");

        BOOST_TEST_MESSAGE(Jan->briefClientInfo());

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        unique_ptr<boost::local_time::local_date_time> ldt(new boost::local_time::local_date_time(time, zone));
        AmountUPtr secondAmount(new Amount(50, 0, PLN));
        LoanSPtr loan(new StandardLoan(move(ldt), move(secondAmount), Jan));
        BOOST_CHECK(Jan->addLoan(loan));

        AmountUPtr amount(new Amount(100, 0, PLN));
        AccountSPtr account(new SavingsAccount(move(amount)));
        BOOST_CHECK(Jan->addAccount(account));

        CardSPtr card(new DebitCard(Jan, account));
        BOOST_CHECK(Jan->addCard(card));

        BOOST_TEST_MESSAGE(Jan->toString());
        BOOST_TEST_MESSAGE("End of ClientStringMethodsTest");
    }

BOOST_AUTO_TEST_SUITE_END()