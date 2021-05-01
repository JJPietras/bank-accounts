//
// Created by student on 11.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "builder/LoanBuilder.h"
#include "builder/ClientBuilder.h"
#include "builder/AccountBuilder.h"
#include "builder/CardBuilder.h"
#include "model/ConsolidationLoan.h"
#include "model/SavingsAccount.h"
#include "model/CurrencyAccount.h"
#include "model/StandardLoan.h"
#include "model/NormalClient.h"
#include "model/DebitCard.h"
#include "model/CreditCard.h"
#include "model/StandardCard.h"
#include "model/ClientType.h"
#include "model/Address.h"
#include "model/Amount.h"
#include "model/Account.h"
#include "model/Client.h"
#include "model/Card.h"
#include "exceptions/ClientExceptions.h"
#include "exceptions/AccountExceptions.h"
#include "exceptions/CardExceptions.h"
#include "exceptions/LoanExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectBuilder)

    BOOST_AUTO_TEST_CASE(ClientBuilderFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientBuilderFunctionalityTest:");

        BOOST_CHECK_THROW(ClientBuilder().setFounds(nullptr), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setIncome(nullptr), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setIncome(make_shared<Amount>(Amount(-1L, -13, PLN))), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setClientType(nullptr), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setAddress(nullptr), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setRegisteredAddress(nullptr), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setFirstName(""), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setLastName(""), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setPersonalID(""), ClientBuilderModificationException);
        BOOST_CHECK_THROW(ClientBuilder().setAccounts(vector<AccountSPtr>()), ClientBuilderModificationException);

        AddressSPtr address1(new Address("Ulica", "Numer"));

        BOOST_CHECK_THROW(ClientBuilder().setLastName("Nazwisko")->setPersonalID("ID")->setRegisteredAddress(address1)->build(), ClientBuilderBuildException);
        BOOST_CHECK_THROW(ClientBuilder().setFirstName("Imie")->setPersonalID("ID")->setRegisteredAddress(address1)->build(), ClientBuilderBuildException);
        BOOST_CHECK_THROW(ClientBuilder().setFirstName("Imie")->setLastName("Nazwisko")->setRegisteredAddress(address1)->build(), ClientBuilderBuildException);
        BOOST_CHECK_THROW(ClientBuilder().setFirstName("Imie")->setLastName("Nazwisko")->setPersonalID("ID")->build(), ClientBuilderBuildException);

        ClientBuilderSPtr builder(new ClientBuilder());

        ClientSPtr client1 = builder
                ->setFirstName("Jan")
                ->setLastName("Kowalski")
                ->setPersonalID("23892387")
                ->setRegisteredAddress(address1)
                ->build();

        BOOST_CHECK_THROW(builder->build(), ClientBuilderBuildException);

        BOOST_CHECK_EQUAL(client1->getFirstName(), "Jan");
        BOOST_CHECK_EQUAL(client1->getLastName(), "Kowalski");
        BOOST_CHECK_EQUAL(client1->getPersonalID(), "23892387");
        BOOST_CHECK_EQUAL(client1->getRegisteredAddress()->getNumber(), address1->getNumber());
        BOOST_CHECK_EQUAL(*client1->getIncome()[EUR], Amount(0L, 0, EUR)); // to pole nie powinno zostać zmienione

        vector<AccountSPtr> accounts1 = {AccountSPtr(new SavingsAccount(AmountUPtr(new Amount(60L, 0, PLN))))};

        vector<AmountUPtr> maxAmounts;
        maxAmounts.emplace_back(AmountUPtr(new Amount(200L, 0, PLN)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(200L, 0, EUR)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(200L, 0, GBP)));
        maxAmounts.emplace_back(AmountUPtr(new Amount(200L, 0, USD)));

        ClientSPtr client2 = builder
                ->setFirstName("Szymon")
                ->setLastName("Kowalski")
                ->setPersonalID("45643456")
                ->setRegisteredAddress(address1)
                ->setAddress(make_shared<Address>(Address("Zgierska", "13")))
                ->setIncome(make_shared<Amount>(Amount(1L, 0, PLN)))
                ->setIncome(make_shared<Amount>(Amount(2L, 0, EUR)))
                ->setIncome(make_shared<Amount>(Amount(3L, 0, GBP)))
                ->setIncome(make_shared<Amount>(Amount(4L, 0, USD)))
                ->setFounds(make_shared<Amount>(Amount(5L, 0, PLN)))
                ->setFounds(make_shared<Amount>(Amount(6L, 0, EUR)))
                ->setFounds(make_shared<Amount>(Amount(7L, 0, GBP)))
                ->setFounds(make_shared<Amount>(Amount(8L, 0, USD)))
                ->setCreditworthiness(50)
                ->setOwnerOfCompany(true)
                ->setClientType(ClientTypeSPtr(new NormalClient(0.95f, 12, move(maxAmounts))))
                ->setAccounts(accounts1)
                ->build();

        BOOST_CHECK_EQUAL(client2->getFirstName(), "Szymon");
        BOOST_CHECK_EQUAL(client2->getLastName(), "Kowalski");
        BOOST_CHECK_EQUAL(client2->getPersonalID(), "45643456");
        BOOST_CHECK_EQUAL(client2->getRegisteredAddress()->getNumber(), address1->getNumber());
        BOOST_CHECK_EQUAL(client2->getAddress()->getNumber(), "13");
        BOOST_CHECK_EQUAL(*client2->getIncome()[PLN], Amount(1L, 0, PLN));
        BOOST_CHECK_EQUAL(*client2->getIncome()[EUR], Amount(2L, 0, EUR));
        BOOST_CHECK_EQUAL(*client2->getIncome()[GBP], Amount(3L, 0, GBP));
        BOOST_CHECK_EQUAL(*client2->getIncome()[USD], Amount(4L, 0, USD));
        BOOST_CHECK_EQUAL(*client2->getFounds()[PLN], Amount(5L, 0, PLN));
        BOOST_CHECK_EQUAL(*client2->getFounds()[EUR], Amount(6L, 0, EUR));
        BOOST_CHECK_EQUAL(*client2->getFounds()[GBP], Amount(7L, 0, GBP));
        BOOST_CHECK_EQUAL(*client2->getFounds()[USD], Amount(8L, 0, USD));
        BOOST_CHECK_EQUAL(client2->getCreditworthiness(), 50);
        BOOST_CHECK_EQUAL(client2->isOwnerOfCompany(), true);
        BOOST_CHECK_NO_THROW(dynamic_cast<NormalClient &>(*client2->getClientType()));
        BOOST_CHECK_CLOSE(client2->getClientType()->getServiceCostMultiplier(), 0.95f, 0.1f);
        BOOST_CHECK(client2->getNumberOfAccounts() != 0);
        BOOST_CHECK(!client2->addAccount(accounts1[0])); // Jeśli istnieje identyczne konto, to nie zostanie dodane (czyli zostało pomyślnie dodane podczas tworzenia builderem)

        SavingsAccountSPtr sameAccount(new SavingsAccount(AmountUPtr(new Amount(60L, 0, PLN))));
        vector<AccountSPtr> accounts2 = {sameAccount, sameAccount};
        BOOST_CHECK_THROW(builder->setAccounts(accounts2), ClientBuilderModificationException);

        BOOST_TEST_MESSAGE(client2->toString());

        BOOST_TEST_MESSAGE("End of ClientBuilderFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(AccountBuilderFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of AccountBuilderFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        ClientSPtr client2(new Client("Tom", "Kowalski", "456345556", AddressUPtr(new Address("Piotrkowska", "1"))));
        ClientSPtr client3(new Client("Andrew", "Kowalski", "987987786", AddressUPtr(new Address("Piotrkowska", "1"))));

        BOOST_CHECK_THROW(AccountBuilder().setMaxOwners(nullptr), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().setMaxOwners(make_shared<short>(1)), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().setAmount(nullptr), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().setAmount(make_shared<Amount>(Amount(-9L, -30, PLN))), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().setOpeningTime(nullptr), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().setOwners(vector<ClientSPtr>()), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().setAccountType(nullptr), AccountBuilderModificationException);

        vector<ClientSPtr> clients{client1};
        vector<ClientSPtr> clients1{client1, client1};
        vector<ClientSPtr> clients2{client1, client2};
        vector<ClientSPtr> clients3{client1, client2, client3};

        BOOST_CHECK_THROW(AccountBuilder().setOwners(clients1), AccountBuilderModificationException);
        BOOST_CHECK_THROW(AccountBuilder().build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setAmount(make_shared<Amount>(Amount(5L, 30, PLN)))->build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setAccountType(make_shared<AccountEnum>(SavingsAccountType))->build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setMaxOwners(make_shared<short>(2))->setAccountType(make_shared<AccountEnum>(JointAccountType))->setOwners(clients3)->build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setAccountType(make_shared<AccountEnum>(SavingsAccountType))->setOwners(clients2)->build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setAccountType(make_shared<AccountEnum>(CurrencyAccountType))->setOwners(clients2)->build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setAccountType(make_shared<AccountEnum>(JointAccountType))->setAmount(make_shared<Amount>(Amount(2L, 0, USD)))->build(), AccountBuilderBuildException);
        BOOST_CHECK_THROW(AccountBuilder().setAccountType(make_shared<AccountEnum>(SavingsAccountType))->setAmount(make_shared<Amount>(Amount(2L, 0, USD)))->build(), AccountBuilderBuildException);

        AccountBuilderUPtr builder(new AccountBuilder());

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        shared_ptr<boost::local_time::local_date_time> ldt(new boost::local_time::local_date_time(time, zone));

        AccountSPtr account = builder
                ->setAccountType(make_shared<AccountEnum>(SavingsAccountType))
                ->setAmount(make_shared<Amount>(Amount(2L, 5, PLN)))
                ->setOpeningTime(ldt)
                ->setOwners(clients)
                ->build();

        BOOST_CHECK_THROW(builder->build(), AccountBuilderBuildException);

        BOOST_CHECK_EQUAL(account->getOwners()[0], client1);
        BOOST_CHECK(*account->getOpeningTime() == *ldt);
        BOOST_CHECK_EQUAL(account->getCurrency(), PLN);
        BOOST_CHECK_EQUAL(*account->getAmount(), Amount(2L, 5, PLN));
        BOOST_CHECK(account->getClosureTime() == nullptr);
        BOOST_CHECK_NO_THROW(dynamic_cast<SavingsAccount &>(*account));

        BOOST_TEST_MESSAGE("End of AccountBuilderFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(CardBuilderFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CardBuilderFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        AccountSPtr account1(new CurrencyAccount(AmountUPtr(new Amount(60L, 0, GBP))));
        AccountSPtr account2(new SavingsAccount(AmountUPtr(new Amount(60L, 0, PLN))));
        AmountSPtr amount1(new Amount(20L, 50, PLN));
        AmountUPtr amount2(new Amount(20L, 50, GBP));
        AmountUPtr amount3(new Amount(20L, 50, GBP));
        AmountSPtr amount4(new Amount(10L, 0, GBP));

        BOOST_CHECK_THROW(CardBuilder().setCardClass(nullptr), CardBuilderModificationException);
        BOOST_CHECK_THROW(CardBuilder().setBalance(nullptr), CardBuilderModificationException);
        BOOST_CHECK_THROW(CardBuilder().setBalance(make_shared<Amount>(Amount(-9L, -30, PLN))), CardBuilderModificationException);
        BOOST_CHECK_THROW(CardBuilder().setOwner(nullptr), CardBuilderModificationException);
        BOOST_CHECK_THROW(CardBuilder().setAccount(nullptr), CardBuilderModificationException);
        BOOST_CHECK_THROW(CardBuilder().setCardType(nullptr), CardBuilderModificationException);

        BOOST_CHECK_THROW(CardBuilder().setCardType(make_shared<CardEnum>(DebitCardType))->setOwner(client1)->build(), CardBuilderBuildException);
        BOOST_CHECK_THROW(CardBuilder().setCardType(make_shared<CardEnum>(DebitCardType))->setAccount(account1)->build(), CardBuilderBuildException);
        BOOST_CHECK_THROW(CardBuilder().setAccount(account1)->setOwner(client1)->build(), CardBuilderBuildException);

        BOOST_CHECK_THROW(CardBuilder().setCardType(make_shared<CardEnum>(CreditCardType))->setAccount(account2)->setOwner(client1)->build(), CardBuilderBuildException);
        BOOST_CHECK_THROW(CardBuilder().setCardType(make_shared<CardEnum>(CreditCardType))->setAccount(account1)->setOwner(client1)->setBalance(amount1)->build(), CardBuilderBuildException);

        CardBuilderSPtr builder(new CardBuilder());
        CardSPtr card = builder
                ->setCardClass(make_shared<StandardCard>(move(amount2), move(amount3)))
                ->setOwner(client1)
                ->setAccount(account1)
                ->setBalance(amount4)
                ->setCardType(make_shared<CardEnum>(CreditCardType))
                ->build();

        BOOST_CHECK_THROW(builder->build(), CardBuilderBuildException);

        BOOST_CHECK_EQUAL(card->getOwner(), client1);
        BOOST_CHECK_EQUAL(card->getAccount(), account1);
        BOOST_CHECK_EQUAL(*card->getAccount()->getAmount(), Amount(60L, 0, GBP));

        BOOST_CHECK_NO_THROW(dynamic_cast<StandardCard&>(*card->getCardClass()));
        BOOST_CHECK_NO_THROW(dynamic_cast<CreditCard&>(*card));

        CreditCardSPtr creditCard = dynamic_pointer_cast<CreditCard>(card);

        BOOST_CHECK_EQUAL(*creditCard->getBalance(), Amount(10L, 0, GBP));

        BOOST_TEST_MESSAGE("End of CardBuilderFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(LoanBuilderFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of LoanBuilderFunctionalityTest:");

        BOOST_CHECK_THROW(LoanBuilder().setBorrowDate(nullptr), LoanBuilderModificationException);
        BOOST_CHECK_THROW(LoanBuilder().setBorrowedAmount(nullptr), LoanBuilderModificationException);
        BOOST_CHECK_THROW(LoanBuilder().setBorrower(nullptr), LoanBuilderModificationException);
        BOOST_CHECK_THROW(LoanBuilder().setLoanType(nullptr), LoanBuilderModificationException);
        BOOST_CHECK_THROW(LoanBuilder().setReturnedAmount(nullptr), LoanBuilderModificationException);
        BOOST_CHECK_THROW(LoanBuilder().setReturnedAmount(make_shared<Amount>(Amount(-9L, -30, PLN))), LoanBuilderModificationException);
        BOOST_CHECK_THROW(LoanBuilder().setConsolidatedLoans(vector<StandardLoanSPtr>()), LoanBuilderModificationException);

        ClientSPtr client(new Client("Jan", "Kowalski", "48375325", AddressUPtr(new Address("Chmielna", "4"))));
        AmountSPtr amount1(new Amount(50L, 0, PLN));
        AmountUPtr amount2(new Amount(50L, 0, PLN));
        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        shared_ptr<boost::local_time::local_date_time> time1(new boost::local_time::local_date_time(time, zone));
        unique_ptr<boost::local_time::local_date_time> time2(new boost::local_time::local_date_time(time, zone));

        BOOST_CHECK_THROW(LoanBuilder().setBorrowedAmount(amount1)->setBorrower(client)->setLoanType(make_shared<LoanEnum>(StandardLoanType))->build(), LoanBuilderBuildException);
        BOOST_CHECK_THROW(LoanBuilder().setBorrowDate(time1)->setBorrower(client)->setLoanType(make_shared<LoanEnum>(StandardLoanType))->build(), LoanBuilderBuildException);
        BOOST_CHECK_THROW(LoanBuilder().setBorrowDate(time1)->setBorrowedAmount(amount1)->setLoanType(make_shared<LoanEnum>(StandardLoanType))->build(), LoanBuilderBuildException);
        BOOST_CHECK_THROW(LoanBuilder().setBorrowedAmount(amount1)->setBorrowDate(time1)->setBorrowedAmount(amount1)->build(), LoanBuilderBuildException);

        LoanBuilderSPtr builder(new LoanBuilder());

        LoanSPtr loan1 = builder
                ->setBorrowDate(time1)
                ->setBorrowedAmount(amount1)
                ->setBorrower(client)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->build();

        LoanSPtr loan2 = builder
                ->setBorrowDate(time1)
                ->setBorrowedAmount(amount1)
                ->setBorrower(client)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->build();

        BOOST_CHECK_THROW(builder->build(), LoanBuilderBuildException);

        BOOST_CHECK_EQUAL(*loan1->getBorrowDate(), *time1);
        BOOST_CHECK_EQUAL(*loan1->getBorrowedAmount(), *amount1);
        BOOST_CHECK_EQUAL(loan1->getBorrower(), client);
        BOOST_CHECK_NO_THROW(dynamic_cast<StandardLoan &>(*loan1));

        vector<StandardLoanSPtr> loans0 = {make_shared<StandardLoan>(move(time2), move(amount2), client)};

        BOOST_CHECK_THROW(builder->setConsolidatedLoans(loans0), LoanBuilderModificationException);

        vector<StandardLoanSPtr> loans1;
        loans1.emplace_back(dynamic_pointer_cast<StandardLoan>(loan1));
        loans1.emplace_back(dynamic_pointer_cast<StandardLoan>(loan1));

        BOOST_CHECK_THROW(builder->setConsolidatedLoans(loans1), LoanBuilderModificationException);

        vector<StandardLoanSPtr> loans2;
        loans2.emplace_back(dynamic_pointer_cast<StandardLoan>(loan1));
        loans2.emplace_back(dynamic_pointer_cast<StandardLoan>(loan2));

        BOOST_CHECK_THROW(builder->setBorrowDate(time1)->setBorrowedAmount(amount1)->setBorrower(client)->setLoanType(make_shared<LoanEnum>(ConsolidationLoanType))->build(),
                          LoanBuilderBuildException);

        LoanSPtr loan3 = builder
                ->setBorrowDate(time1)
                ->setBorrowedAmount(amount1)
                ->setBorrower(client)
                ->setConsolidatedLoans(loans2)
                ->setReturnedAmount(make_shared<Amount>(*amount1 + *amount1 + *amount1))
                ->setLoanType(make_shared<LoanEnum>(ConsolidationLoanType))
                ->build();

        BOOST_CHECK_EQUAL(*loan3->getBorrowDate(), *time1);
        BOOST_CHECK_EQUAL(*loan3->getBorrowedAmount(), *amount1 + *amount1);
        BOOST_CHECK_EQUAL(loan3->getBorrower(), client);
        BOOST_CHECK_NO_THROW(dynamic_cast<ConsolidationLoan &>(*loan3));
        BOOST_CHECK(loan3->getPaymentDate() != nullptr);

        BOOST_CHECK_THROW(builder->setBorrowDate(time1)->setBorrowedAmount(amount1)->setBorrower(client)->setReturnedAmount(make_shared<Amount>(Amount(1L, 0, GBP)))->build(),
                          LoanBuilderBuildException);

        AmountSPtr amount3(new Amount(9L, 5, USD));
        builder->setBorrowedAmount(amount1)->setBorrowDate(time1)->setBorrower(client)->setLoanType(make_shared<LoanEnum>(StandardLoanType))->setReturnedAmount(amount3);
        BOOST_CHECK_THROW(builder->build(), LoanBuilderBuildException);

        BOOST_TEST_MESSAGE(loan3->toString());

        BOOST_TEST_MESSAGE("End of LoanBuilderFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()