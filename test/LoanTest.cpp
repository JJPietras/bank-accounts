//
// Created by student on 03.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "model/ConsolidationLoan.h"
#include "model/Mortgage.h"
#include "model/StandardLoan.h"
#include "model/Client.h"
#include "model/Address.h"
#include "model/Amount.h"
#include "exceptions/LoanExceptions.h"
#include "exceptions/AmountExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectLoan)

    ClientSPtr client(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
    boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
    boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
    unique_ptr<boost::local_time::local_date_time> ldt(new boost::local_time::local_date_time(time, zone));

    BOOST_AUTO_TEST_CASE(LoanConstructorFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of LoanConstructorFunctionalityTest:");

        BOOST_CHECK_THROW(Mortgage(nullptr, AmountUPtr(new Amount(0L, 1, EUR)), client), LoanConstructionException);
        BOOST_CHECK_THROW(Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), nullptr, client), LoanConstructionException);

        BOOST_CHECK_THROW(Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(9L, 99, EUR)), client), LoanConstructionException);

        BOOST_CHECK_THROW(Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(0L, 1, EUR)), nullptr), LoanConstructionException);

        MortgageUPtr LOAN(new Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(10L, 0, EUR)), client));

        BOOST_CHECK_EQUAL(LOAN->getBorrower(), client);
        BOOST_CHECK_EQUAL(LOAN->getCurrencyType(), EUR);
        BOOST_CHECK_EQUAL(*LOAN->getBorrowedAmount(), Amount(10L, 0, EUR));
        BOOST_CHECK_EQUAL(*LOAN->getReturnedAmount(), Amount(0L, 0, EUR));
        BOOST_CHECK_EQUAL(*LOAN->getBorrowDate(), *ldt);
        BOOST_CHECK(LOAN->getPaymentDate() == nullptr);

        BOOST_TEST_MESSAGE("End of LoanConstructorFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(LoanOtherFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of LoanOtherFunctionalityTest:");

        MortgageUPtr LOAN(new Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(10L, 0, EUR)), client));

        BOOST_CHECK_EQUAL(*LOAN->getBorrower()->getFounds()[EUR], Amount(0L, 0, EUR));

        BOOST_CHECK(!LOAN->returnMoney(make_shared<Amount>(Amount(5L, 0, EUR))));
        BOOST_CHECK_EQUAL(*LOAN->getReturnedAmount(), Amount(5L, 0, EUR));
        BOOST_CHECK(LOAN->returnMoney(make_shared<Amount>(Amount(10L, 0, EUR))));
        BOOST_CHECK(*LOAN->getBorrower()->getFounds()[EUR] == Amount(1L, 0, EUR));
        BOOST_CHECK(LOAN->getPaymentDate() != nullptr && !LOAN->getPaymentDate()->is_not_a_date_time());

        BOOST_TEST_MESSAGE("End of LoanOtherFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(MortgageFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of MortgageFunctionalityTest:");

        MortgageUPtr LOAN(new Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(10L, 0, EUR)), client));

        BOOST_CHECK_EQUAL(LOAN->calculatePercentage(), 1.40f);
        BOOST_CHECK_EQUAL(*LOAN->getLoanCost(), Amount(14L, 00, EUR));

        MortgageUPtr LOAN2(new Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(100001L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN2->calculatePercentage(), 1.34f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN2->getLoanCost(), Amount(134001L, 34, EUR));
        BOOST_TEST_MESSAGE(LOAN2->briefLoanInfo());

        MortgageUPtr LOAN3(new Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(900001L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN3->calculatePercentage(), 1.26f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN3->getLoanCost(), Amount(1134001L, 26, EUR));
        BOOST_TEST_MESSAGE(LOAN3->briefLoanInfo());

        MortgageUPtr LOAN4(new Mortgage(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(1000001L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN4->calculatePercentage(), 1.15f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN4->getLoanCost(), Amount(1150001L, 15, EUR));
        BOOST_TEST_MESSAGE(LOAN4->briefLoanInfo());
        BOOST_TEST_MESSAGE(LOAN4->toString());
        BOOST_CHECK(LOAN4->returnMoney(make_shared<Amount>(Amount(1150001L, 15, EUR))));
        BOOST_TEST_MESSAGE(LOAN4->briefLoanInfo());
        BOOST_TEST_MESSAGE(LOAN4->toString());

        BOOST_TEST_MESSAGE("End of MortgageFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(StandardLoanFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of MortgageFunctionalityTest:");

        StandardLoanUPtr LOAN(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(100L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN->calculatePercentage(), 1.15f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN->getLoanCost(), Amount(115L, 0, EUR));

        StandardLoanUPtr LOAN2(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(1000L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN2->calculatePercentage(), 1.2f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN2->getLoanCost(), Amount(1200L, 0, EUR));

        StandardLoanUPtr LOAN3(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(10000L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN3->calculatePercentage(), 1.25f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN3->getLoanCost(), Amount(12500L, 0, EUR));

        StandardLoanUPtr LOAN4(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(100000L, 0, EUR)), client));

        BOOST_CHECK_CLOSE(LOAN4->calculatePercentage(), 1.35f, 0.1f);
        BOOST_CHECK_EQUAL(*LOAN4->getLoanCost(), Amount(135000L, 0, EUR));

        BOOST_TEST_MESSAGE(LOAN4->briefLoanInfo());
        BOOST_TEST_MESSAGE(LOAN4->toString());

        BOOST_TEST_MESSAGE("End of MortgageFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(ConsolidationLoanFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of ConsolidationLoanFunctionalityTest:");

        StandardLoanSPtr LOAN1(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(100L, 0, EUR)), client));

        StandardLoanSPtr LOAN2(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(200L, 0, PLN)), client));

        ClientSPtr client2 = make_shared<Client>("Robert", "Piotrowski", "23455423", AddressUPtr(new Address("Piotrkowska", "1")));
        StandardLoanSPtr LOAN3(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(100L, 0, EUR)), client2));

        StandardLoanSPtr LOAN4(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(1000L, 0, EUR)), client));

        vector<StandardLoanSPtr> loans1 = {LOAN1, LOAN2};
        BOOST_CHECK_THROW(ConsolidationLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), loans1), ConsolidationLoanMethodException);

        vector<StandardLoanSPtr> loans2 = {LOAN1, LOAN3};
        BOOST_CHECK_THROW(ConsolidationLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), loans2), ConsolidationLoanConstructionException);

        vector<StandardLoanSPtr> loans3 = {LOAN1, LOAN1};
        BOOST_CHECK_THROW(ConsolidationLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), loans3), ConsolidationLoanConstructionException);

        BOOST_CHECK_THROW(ConsolidationLoan(nullptr, loans2), logic_error);

        LOAN1->returnMoney(make_shared<Amount>(Amount(50L, 0, EUR)));
        LOAN4->returnMoney(make_shared<Amount>(Amount(300L, 0, EUR)));

        vector<StandardLoanSPtr> loans5 = {LOAN1, LOAN4};
        ConsolidationLoanUPtr CLOAN1(new ConsolidationLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), loans5));

        BOOST_CHECK_EQUAL(*CLOAN1->getBorrowedAmount(), Amount(1100L, 0, EUR));
        BOOST_CHECK_CLOSE(CLOAN1->calculatePercentage(), 1.175f, 0.1f);
        BOOST_CHECK_EQUAL(CLOAN1->getBorrower(), client);
        BOOST_CHECK_EQUAL(CLOAN1->getCurrencyType(), EUR);
        BOOST_CHECK(*CLOAN1->getLoanCost() < (*LOAN1->getLoanCost() + *LOAN4->getLoanCost()));
        BOOST_CHECK_EQUAL(*CLOAN1->getReturnedAmount(), Amount(350L, 0, EUR));
        BOOST_CHECK(CLOAN1->getPaymentDate() == nullptr);
        BOOST_TEST_MESSAGE(CLOAN1->briefLoanInfo());
        BOOST_TEST_MESSAGE(CLOAN1->toString());

        vector<StandardLoanSPtr> loans6 = {LOAN1};
        vector<StandardLoanSPtr> loans7;

        BOOST_CHECK(loans7.empty());

        BOOST_CHECK_THROW(ConsolidationLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), loans6), ConsolidationLoanMethodException);
        BOOST_CHECK_THROW(ConsolidationLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), loans7), ConsolidationLoanMethodException);

        BOOST_TEST_MESSAGE("End of ConsolidationLoanFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()