//
// Created by student on 01.01.2020.
//

#include <boost/test/unit_test.hpp>

#include "model/Address.h"
#include "exceptions/AddressExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectAddress)

    BOOST_AUTO_TEST_CASE(AddressTest) {
        BOOST_TEST_MESSAGE("Beginning of AddressTest:");

        BOOST_CHECK_THROW(Address("Polna", ""), AddressException);
        BOOST_CHECK_THROW(Address("  ", "4"), AddressException);
        BOOST_CHECK_THROW(Address("", "    "), AddressException);

        AddressUPtr AD1(new Address("  Wiosny Ludow ", "34 / 2     "));

        BOOST_CHECK_EQUAL(AD1->getStreet(), "Wiosny Ludow");
        BOOST_CHECK_EQUAL(AD1->getNumber(), "34 / 2");

        BOOST_CHECK_EQUAL(AD1->toString(), "Wiosny Ludow 34 / 2");

        BOOST_TEST_MESSAGE("End of AddressTest");
    }

BOOST_AUTO_TEST_SUITE_END()