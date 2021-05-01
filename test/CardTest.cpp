//
// Created by student on 03.01.2020.
//

#include <boost/test/unit_test.hpp>

#include "model/Address.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Card.h"
#include "model/SilverCard.h"
#include "model/DebitCard.h"
#include "model/CreditCard.h"
#include "model/CurrencyAccount.h"
#include "model/StandardCard.h"
#include "exceptions/CardExceptions.h"
#include "exceptions/AmountExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectCard)

    ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
    ClientSPtr client2(new Client("Tom", "Kowalski", "12312323", AddressUPtr(new Address("Piotrkowska", "4"))));
    ClientSPtr client3(new Client("Rob", "Kowalski", "90312323", AddressUPtr(new Address("Piotrkowska", "19"))));

    CurrencyAccountSPtr account1(new CurrencyAccount(AmountUPtr(new Amount(10L, 0, EUR))));
    CurrencyAccountSPtr account2(new CurrencyAccount(AmountUPtr(new Amount(10L, 0, EUR))));
    CurrencyAccountSPtr account3(new CurrencyAccount(AmountUPtr(new Amount(10L, 0, GBP))));

    BOOST_AUTO_TEST_CASE(CardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CardFunctionalityTest:");

        BOOST_CHECK_THROW(CreditCard(nullptr, account1), CardConstructionException);
        BOOST_CHECK_THROW(CreditCard(client1, nullptr), CardConstructionException);

        CreditCardSPtr C1(new CreditCard(client1, account1));

        BOOST_CHECK_THROW(C1->setCardClass(nullptr), CardModificationException);
        BOOST_CHECK_THROW(C1->makePayment(nullptr), CardMethodException);
        BOOST_CHECK_THROW(C1->makePayment(AmountUPtr(new Amount(2L, 0, PLN))), CardMethodException);
        BOOST_CHECK_THROW(C1->makePayment(AmountUPtr(new Amount(-1L, 0, EUR))), CardMethodException);
        BOOST_CHECK(!C1->makePayment(AmountUPtr(new Amount(200L, 0, EUR))));
        BOOST_CHECK(C1->makePayment(AmountUPtr(new Amount(2L, 0, EUR))));
        BOOST_CHECK_EQUAL(*account1->getAmount(), Amount(8L, 0, EUR));
        BOOST_CHECK(C1->getUuid() != nullptr);
        BOOST_CHECK_EQUAL(C1->getOwner(), client1);
        BOOST_CHECK_EQUAL(C1->getAccount(), account1);


        SilverCardSPtr cardClass(new SilverCard(AmountUPtr(new Amount(5L, 0, PLN)), AmountUPtr(new Amount(5L, 0, PLN))));
        BOOST_CHECK_NO_THROW(C1->setCardClass(cardClass));
        BOOST_CHECK_THROW(*C1->calculateWithdrawCost(), CardClassMethodException);
        account1->addOwner(client1);
        BOOST_CHECK_EQUAL(*C1->calculateWithdrawCost(), *cardClass->calculateWithdrawCost(account1));

        BOOST_TEST_MESSAGE("End of CardFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(DebitCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of DebitCardFunctionalityTest:");

        DebitCardSPtr C2(new DebitCard(client2, account2));

        C2->setCardClass(make_shared<StandardCard>(AmountUPtr(new Amount(5L, 00, EUR)), AmountUPtr(new Amount(5L, 00, EUR))));

        BOOST_CHECK_THROW(C2->payIntoAccount(nullptr), CardMethodException);
        BOOST_CHECK_THROW(C2->payIntoAccount(AmountUPtr(new Amount(-100L, 0, EUR))), CardMethodException);
        BOOST_CHECK(C2->payIntoAccount(AmountUPtr(new Amount(100L, 0, EUR))));
        BOOST_CHECK_EQUAL(*C2->getAccount()->getAmount(), Amount(105L, 0, EUR));

        BOOST_TEST_MESSAGE(C2->briefCardInfo());
        BOOST_TEST_MESSAGE(C2->toString());

        BOOST_TEST_MESSAGE("End of DebitCardFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(CreditCardFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CreditCardFunctionalityTest:");

        CreditCardSPtr C3(new CreditCard(client3, account3));

        BOOST_CHECK(C3->makePayment(AmountUPtr(new Amount(2L, 0, GBP))));
        BOOST_CHECK_EQUAL(*C3->getAmount(), Amount(8L, 0, GBP));
        BOOST_CHECK_EQUAL(*C3->getBalance(), Amount(2L, 0, GBP));

        BOOST_CHECK_THROW(C3->payBack(AmountUPtr(new Amount(2L, 0, PLN))), CardMethodException);
        BOOST_CHECK_THROW(C3->payBack(AmountUPtr(new Amount(-2L, 0, GBP))), CardMethodException);
        BOOST_CHECK_THROW(C3->payBack(AmountUPtr(nullptr)), CardMethodException);
        BOOST_CHECK_THROW(C3->makePayment(AmountUPtr(new Amount(2L, 0, PLN))), CardMethodException);

        BOOST_CHECK(!C3->payBack(AmountUPtr(new Amount(1L, 0, GBP))));
        BOOST_CHECK_EQUAL(*C3->getBalance(), Amount(1L, 0, GBP));
        BOOST_CHECK_EQUAL(*C3->getAmount(), Amount(9L, 0, GBP));
        BOOST_CHECK(C3->payBack(AmountUPtr(new Amount(1L, 0, GBP))));
        BOOST_CHECK_EQUAL(*C3->getBalance(), Amount(0L, 0, GBP));
        BOOST_CHECK_EQUAL(*C3->getAmount(), Amount(10L, 0, GBP));
        BOOST_CHECK_EQUAL(*C3->getOwner()->getFounds()[GBP], Amount(0L, 0, GBP));

        BOOST_CHECK(C3->makePayment(AmountUPtr(new Amount(2L, 0, GBP))));
        BOOST_CHECK(C3->payBack(AmountUPtr(new Amount(3L, 0, GBP))));
        BOOST_CHECK_EQUAL(*C3->getBalance(), Amount(0L, 0, GBP));
        BOOST_CHECK_EQUAL(*C3->getAmount(), Amount(10L, 0, GBP));
        BOOST_CHECK_EQUAL(*C3->getOwner()->getFounds()[GBP], Amount(1L, 0, GBP));

        BOOST_TEST_MESSAGE(C3->briefCardInfo());
        BOOST_TEST_MESSAGE(C3->toString());

        BOOST_TEST_MESSAGE("End of CreditCardFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()