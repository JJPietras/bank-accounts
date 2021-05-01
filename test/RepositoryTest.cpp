//
// Created by student on 07.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/date_time/local_time/local_date_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "repository/ClientRepository.h"
#include "repository/LoanRepository.h"
#include "repository/CardRepository.h"
#include "repository/AccountRepository.h"
#include "model/NormalClient.h"
#include "model/StandardLoan.h"
#include "model/DebitCard.h"
#include "model/SavingsAccount.h"
#include "model/Client.h"
#include "model/Address.h"
#include "model/Amount.h"
#include "exceptions/ClientExceptions.h"
#include "exceptions/AccountExceptions.h"
#include "exceptions/LoanExceptions.h"
#include "exceptions/CardExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectLoan)

    BOOST_AUTO_TEST_CASE(ClientRepositoryFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientRepositoryFunctionalityTest:");
        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        ClientSPtr client2(new Client("Tom", "Kowalski", "12312323", AddressUPtr(new Address("Piotrkowska", "4"))));

        ClientRepositorySPtr clientRepository(new ClientRepository());

        clientRepository->add(client1);
        BOOST_CHECK_THROW(clientRepository->add(client1), ClientRepositoryException);
        clientRepository->remove(client1);
        BOOST_CHECK_THROW(clientRepository->remove(client1), ClientRepositoryException);
        BOOST_CHECK_THROW(clientRepository->add(nullptr), ClientRepositoryException);
        BOOST_CHECK_THROW(clientRepository->remove(nullptr), ClientRepositoryException);
        clientRepository->add(client1);

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(2L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(2L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(2L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(2L, 0, USD)));
        vector<AmountUPtr> maxAmounts2;
        maxAmounts2.emplace_back(AmountUPtr(new Amount(2L, 0, PLN)));
        maxAmounts2.emplace_back(AmountUPtr(new Amount(2L, 0, EUR)));
        maxAmounts2.emplace_back(AmountUPtr(new Amount(2L, 0, GBP)));
        maxAmounts2.emplace_back(AmountUPtr(new Amount(2L, 0, USD)));

        BOOST_CHECK_THROW(clientRepository->changeClientType(nullptr, NormalClientType, 1.0f, 1, move(maxAmounts)), ClientRepositoryException);
        BOOST_CHECK_EQUAL(clientRepository->find([](const ClientSPtr &client) { return client->getAddress()->getNumber() == "1"; }), client1);
        BOOST_CHECK_EQUAL(clientRepository->find([](const ClientSPtr &client) { return client->getAddress()->getNumber() == "2"; }), nullptr);
        clientRepository->changeClientType(client1, NormalClientType, 1.0f, 1, move(maxAmounts2));
        BOOST_CHECK_NO_THROW(dynamic_cast<NormalClient&>(*client1->getClientType()));
        BOOST_CHECK(clientRepository->getAll().size() == 1);
        BOOST_CHECK_NO_THROW(clientRepository->add(client2));
        BOOST_CHECK(clientRepository->getAll().size() == 2);

        BOOST_TEST_MESSAGE("End of ClientRepositoryFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(AcountRepositoryFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of AcountRepositoryFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));

        AccountRepositorySPtr accountRepository(new AccountRepository());

        AccountSPtr account1(new SavingsAccount(AmountUPtr(new Amount(200L, 30, PLN))));
        AccountSPtr account2(new SavingsAccount(AmountUPtr(new Amount(200L, 30, PLN))));

        accountRepository->add(account1);

        BOOST_CHECK_THROW(accountRepository->add(account1), AccountRepositoryException);
        accountRepository->remove(account1);
        BOOST_CHECK_THROW(accountRepository->remove(account1), AccountRepositoryException);
        BOOST_CHECK_THROW(accountRepository->add(nullptr), AccountRepositoryException);
        BOOST_CHECK_THROW(accountRepository->remove(nullptr), AccountRepositoryException);
        account1->addOwner(client1);
        accountRepository->add(account1);
        BOOST_CHECK_EQUAL(accountRepository->find([](const AccountSPtr &account) { return account->getOwners()[0]->getAddress()->getNumber() == "1"; }), account1);
        BOOST_CHECK_EQUAL(accountRepository->find([](const AccountSPtr &account) { return account->getOwners()[0]->getAddress()->getNumber() == "2"; }), nullptr);
        BOOST_CHECK(accountRepository->getAll().size() == 1);
        BOOST_CHECK_NO_THROW(accountRepository->add(account2));
        BOOST_CHECK(accountRepository->getAll().size() == 2);

        BOOST_TEST_MESSAGE("End of AcountRepositoryFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(CardRepositoryFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CardRepositoryFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));

        AccountSPtr account1(new SavingsAccount(AmountUPtr(new Amount(200L, 30, PLN))));

        CardRepositorySPtr cardRepository(new CardRepository());

        CardSPtr card1(new DebitCard(client1, account1));
        CardSPtr card2(new DebitCard(client1, account1));

        cardRepository->add(card1);

        BOOST_CHECK_THROW(cardRepository->add(card1), CardRepositoryException);
        cardRepository->remove(card1);
        BOOST_CHECK_THROW(cardRepository->remove(card1), CardRepositoryException);
        BOOST_CHECK_THROW(cardRepository->add(nullptr), CardRepositoryException);
        BOOST_CHECK_THROW(cardRepository->remove(nullptr), CardRepositoryException);
        cardRepository->add(card1);
        BOOST_CHECK_EQUAL(cardRepository->find([](const CardSPtr &card) { return card->getOwner()->getAddress()->getNumber() == "1"; }), card1);
        BOOST_CHECK_EQUAL(cardRepository->find([](const CardSPtr &card) { return card->getOwner()->getAddress()->getNumber() == "2"; }), nullptr);
        BOOST_CHECK(cardRepository->getAll().size() == 1);
        BOOST_CHECK_NO_THROW(cardRepository->add(card2));
        BOOST_CHECK(cardRepository->getAll().size() == 2);

        BOOST_TEST_MESSAGE("End of CardRepositoryFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(LoanRepositoryFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of LoanRepositoryFunctionalityTest:");

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        unique_ptr<boost::local_time::local_date_time> ldt(new boost::local_time::local_date_time(time, zone));

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        ClientSPtr client2(new Client("Tom", "Kowalski", "12312323", AddressUPtr(new Address("Piotrkowska", "4"))));

        LoanRepositorySPtr loanRepository(new LoanRepository());

        LoanSPtr loan1(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(10L, 5, PLN)), client1));
        LoanSPtr loan2(new StandardLoan(unique_ptr<boost::local_time::local_date_time>(new boost::local_time::local_date_time(*ldt)), AmountUPtr(new Amount(10L, 5, PLN)), client2));

        loanRepository->add(loan1);
        BOOST_CHECK_THROW(loanRepository->add(loan1), LoanRepositoryException);
        loanRepository->remove(loan1);
        BOOST_CHECK_THROW(loanRepository->remove(loan1), LoanRepositoryException);
        BOOST_CHECK_THROW(loanRepository->add(nullptr), LoanRepositoryException);
        BOOST_CHECK_THROW(loanRepository->remove(nullptr), LoanRepositoryException);
        loanRepository->add(loan1);
        BOOST_CHECK_EQUAL(loanRepository->find([](const LoanSPtr &loan) { return loan->getBorrower()->getAddress()->getNumber() == "1"; }), loan1);
        BOOST_CHECK_EQUAL(loanRepository->find([](const LoanSPtr &loan) { return loan->getBorrower()->getAddress()->getNumber() == "2"; }), nullptr);
        BOOST_CHECK(loanRepository->getAll().size() == 1);
        BOOST_CHECK_NO_THROW(loanRepository->add(loan2));
        BOOST_CHECK(loanRepository->getAll().size() == 2);

        BOOST_TEST_MESSAGE("End of LoanRepositoryFunctionalityTest");
    }


BOOST_AUTO_TEST_SUITE_END()
