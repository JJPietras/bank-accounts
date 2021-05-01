//
// Created by student on 29.12.2019.
//

#include <boost/test/unit_test.hpp>

#include "model/Amount.h"
#include "exceptions/AmountExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectAmount)

    AmountUPtr amount(new Amount(300, 20, USD));
    AmountUPtr zero(new Amount(0, 0, USD));
    AmountUPtr coin(new Amount(0, 1, USD));

    BOOST_AUTO_TEST_CASE(AmountExceptionCase) {
        BOOST_TEST_MESSAGE("Beginning of AmountExceptionCase:");

        BOOST_CHECK_THROW(Amount(300, -20, USD), AmountConstructionException);
        BOOST_CHECK_THROW(Amount(-300, 10, USD), AmountConstructionException);
        BOOST_CHECK_THROW(Amount(300, 100, USD), AmountConstructionException);
        BOOST_CHECK_THROW(Amount(30, -100, USD), AmountConstructionException);
        BOOST_CHECK_THROW(Amount(300, -200, USD), AmountConstructionException);

        BOOST_TEST_MESSAGE("End of AmountExceptionCase");
    }

    BOOST_AUTO_TEST_CASE(AmountConstructorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountConstructorTest:");

        BOOST_CHECK_EQUAL(amount->getMainUnit(), 300);
        BOOST_CHECK_EQUAL(amount->getFractionalUnit(), 20);

        BOOST_TEST_MESSAGE("End of AmountConstructorTest");
    }

    BOOST_AUTO_TEST_CASE(AmountToStringTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountToStringTest:");

        AmountUPtr secondAmount(new Amount(-300, -20, USD));

        BOOST_CHECK_EQUAL(amount->toString(), "300.20$");
        BOOST_CHECK_EQUAL(secondAmount->toString(), "-300.20$");

        BOOST_TEST_MESSAGE("End of AmountToStringTest");
    }

    BOOST_AUTO_TEST_CASE(AmountAdditionOperatorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountAdditionOperatorTest:");

        BOOST_CHECK_THROW(*amount + Amount(0, 0, PLN), AmountOperatorException);

        AmountUPtr thirdAmount(new Amount(-300, -20, USD));
        AmountUPtr firstResult(new Amount(*amount + *thirdAmount));
        BOOST_CHECK_EQUAL(firstResult->toString(), "0.00$");

        AmountUPtr fourthAmount(new Amount(-1, -20, USD));
        AmountUPtr fifthAmount(new Amount(0, 30, USD));
        AmountUPtr secondResult(new Amount(*fourthAmount + *fifthAmount));
        BOOST_CHECK_EQUAL(secondResult->toString(), "-0.90$");

        AmountUPtr sixthAmount(new Amount(1, 10, USD));
        AmountUPtr thirdResult(new Amount(*fourthAmount + *sixthAmount));
        BOOST_CHECK_EQUAL(thirdResult->toString(), "-0.10$");

        AmountUPtr seventhAmount(new Amount(2, 10, USD));
        AmountUPtr fourthResult(new Amount(*fourthAmount + *seventhAmount));
        BOOST_CHECK_EQUAL(fourthResult->toString(), "0.90$");

        AmountUPtr eightAmount(new Amount(2, 20, USD));
        AmountUPtr fifthResult(new Amount(*fourthAmount + *eightAmount));
        BOOST_CHECK_EQUAL(fifthResult->toString(), "1.00$");

        AmountUPtr ninthAmount(new Amount(0, -90, USD));
        AmountUPtr sixthResult(new Amount(*fourthAmount + *ninthAmount));
        BOOST_CHECK_EQUAL(sixthResult->toString(), "-2.10$");

        AmountUPtr tenthAmount(new Amount(0, 90, USD));
        AmountUPtr seventhResult(new Amount(*eightAmount + *tenthAmount));
        BOOST_CHECK_EQUAL(seventhResult->toString(), "3.10$");

        BOOST_TEST_MESSAGE("End of AmountAdditionOperatorTest");
    }

    BOOST_AUTO_TEST_CASE(AmountSubtractionOperatorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountSubtractionOperatorTest:");

        BOOST_CHECK_THROW(*amount - Amount(0, 0, PLN), AmountOperatorException);

        AmountUPtr thirdAmount(new Amount(300L, 20, USD));
        AmountUPtr firstResult(new Amount(*amount - *thirdAmount));
        BOOST_CHECK_EQUAL(firstResult->toString(), "0.00$");

        AmountUPtr fourthAmount(new Amount(-1L, -90, USD));
        AmountUPtr fifthAmount(new Amount(0L, 30, USD));
        AmountUPtr secondResult(new Amount(*fourthAmount - *fifthAmount));
        BOOST_CHECK_EQUAL(secondResult->toString(), "-2.20$");

        AmountUPtr sixthAmount(new Amount(1L, 10, USD));
        AmountUPtr thirdResult(new Amount(*fourthAmount - *sixthAmount));
        BOOST_CHECK_EQUAL(thirdResult->toString(), "-3.00$");

        AmountUPtr seventhAmount(new Amount(2L, 10, USD));
        AmountUPtr fourthResult(new Amount(*fourthAmount - *seventhAmount));
        BOOST_CHECK_EQUAL(fourthResult->toString(), "-4.00$");

        AmountUPtr eightAmount(new Amount(-2L, -20, USD));
        AmountUPtr fifthResult(new Amount(*fourthAmount - *eightAmount));
        BOOST_CHECK_EQUAL(fifthResult->toString(), "0.30$");

        AmountUPtr ninthAmount(new Amount(0L, -90, USD));
        AmountUPtr sixthResult(new Amount(*fourthAmount - *ninthAmount));
        BOOST_CHECK_EQUAL(sixthResult->toString(), "-1.00$");

        AmountUPtr tenthAmount(new Amount(0L, 90, USD));
        AmountUPtr seventhResult(new Amount(*fourthAmount - *tenthAmount));
        BOOST_CHECK_EQUAL(seventhResult->toString(), "-2.80$");

        BOOST_TEST_MESSAGE("End of AmountSubtractionOperatorTest");
    }

    BOOST_AUTO_TEST_CASE(AmountMultiplyOperatorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountMultiplyOperatorTest:");

        AmountUPtr AM1(new Amount(-300L, -20, USD));
        float m1 = 3.0f;
        BOOST_CHECK_EQUAL(*AM1 * m1, Amount(-900L, -60, USD));

        double m2 = 1.5;
        BOOST_CHECK_EQUAL(*AM1 * m2, Amount(-450L, -30, USD));

        double m3 = -0.783;
        BOOST_CHECK_EQUAL(*AM1 * m3, Amount(235L, 6, USD));

        double m4 = 0.0;
        BOOST_CHECK_EQUAL(*AM1 * m4, *zero);

        BOOST_TEST_MESSAGE("End of AmountMultiplyOperatorTest");
    }

    BOOST_AUTO_TEST_CASE(AmountEqualityOperatorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountEqualityOperatorTest:");

        BOOST_CHECK_THROW(*amount == Amount(0, 0, PLN), AmountOperatorException);

        AmountUPtr AM1(new Amount(-300L, -20, USD));
        AmountUPtr AM2(new Amount(300L, 20, USD));
        BOOST_CHECK(!(*amount == *AM1));
        BOOST_CHECK(*amount == *AM2);

        BOOST_TEST_MESSAGE("End of AmountEqualityOperatorTest");
    }

    BOOST_AUTO_TEST_CASE(AmountGreaterOperatorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountGreaterOperatorTest:");

        BOOST_CHECK_THROW(*amount > Amount(0, 0, PLN), AmountOperatorException);

        AmountUPtr AM1(new Amount(-300L, -20, USD));
        AmountUPtr AM2(new Amount(300L, 20, USD));
        BOOST_CHECK(*amount > *AM1);
        BOOST_CHECK(!(*AM1 > *amount));
        BOOST_CHECK(!(*AM2 > *amount));
        BOOST_CHECK(!(*amount > *AM2));
        BOOST_CHECK(*zero > *AM1);
        BOOST_CHECK(!(*zero > *AM2));
        BOOST_CHECK(!(*zero > *coin));
        BOOST_CHECK(*coin > *zero);

        BOOST_TEST_MESSAGE("End of AmountGreaterOperatorTest");
    }

    BOOST_AUTO_TEST_CASE(AmountLessOperatorTest) {
        BOOST_TEST_MESSAGE("Beginning of AmountLessOperatorTest:");

        BOOST_CHECK_THROW(*amount < Amount(0, 0, PLN), AmountOperatorException);

        AmountUPtr AM1(new Amount(-300L, -20, USD));
        AmountUPtr AM2(new Amount(300L, 20, USD));
        BOOST_CHECK(!(*amount < *AM1));
        BOOST_CHECK(*AM1 < *amount);
        BOOST_CHECK(!(*AM2 < *amount));
        BOOST_CHECK(!(*amount < *AM2));
        BOOST_CHECK(!(*zero < *AM1));
        BOOST_CHECK(*zero < *AM2);
        BOOST_CHECK(*zero < *coin);
        BOOST_CHECK(!(*coin < *zero));

        BOOST_TEST_MESSAGE("End of AmountLessOperatorTest");
    }

BOOST_AUTO_TEST_SUITE_END()