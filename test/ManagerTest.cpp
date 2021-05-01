//
// Created by student on 11.01.2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "repository/AccountRepository.h"
#include "repository/ClientRepository.h"
#include "repository/CardRepository.h"
#include "repository/LoanRepository.h"
#include "manager/AccountManager.h"
#include "manager/ClientManager.h"
#include "manager/CardManager.h"
#include "manager/LoanManager.h"
#include "builder/AccountBuilder.h"
#include "builder/LoanBuilder.h"
#include "builder/CardBuilder.h"
#include "model/SavingsAccount.h"
#include "model/CurrencyAccount.h"
#include "model/Account.h"
#include "model/CreditCard.h"
#include "model/DebitCard.h"
#include "model/Client.h"
#include "model/Card.h"
#include "model/Loan.h"
#include "model/Address.h"
#include "model/Amount.h"
#include "model/ClientType.h"
#include "model/StandardLoan.h"
#include "enum/LoanRepositoriesEnum.h"
#include "exceptions/ClientExceptions.h"
#include "exceptions/AccountExceptions.h"
#include "exceptions/LoanExceptions.h"
#include "exceptions/CardExceptions.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(TestSuiteCorrectManager)

    BOOST_AUTO_TEST_CASE(ClientManagerFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of ClientManagerFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));

        ClientRepositorySPtr standardClients(new ClientRepository());
        ClientRepositorySPtr formerClients(new ClientRepository());
        ClientRepositorySPtr debtors(new ClientRepository());

        ClientManagerUPtr clientManager(new ClientManager(formerClients, debtors, standardClients));

        BOOST_CHECK_THROW(ClientManager(nullptr, debtors, standardClients), ClientManagerConstructionException);
        BOOST_CHECK_THROW(ClientManager(formerClients, nullptr, standardClients), ClientManagerConstructionException);
        BOOST_CHECK_THROW(ClientManager(formerClients, debtors, nullptr), ClientManagerConstructionException);

        BOOST_CHECK_THROW(clientManager->clientInfo(nullptr), ClientManagerMethodException);
        BOOST_CHECK_THROW(clientManager->changeClientStatus(nullptr, StandardClientsRepo), ClientManagerMethodException);
        BOOST_CHECK_THROW(clientManager->changeClientStatus(client1, StandardClientsRepo), ClientManagerMethodException);

        BOOST_CHECK_NO_THROW(clientManager->addClient(client1, StandardClientsRepo));
        BOOST_CHECK_THROW(clientManager->addClient(client1, StandardClientsRepo), ClientManagerException);
        BOOST_CHECK_NO_THROW(clientManager->changeClientStatus(client1, DebtorsRepo));
        BOOST_CHECK_NO_THROW(clientManager->clientInfo(client1));
        BOOST_TEST_MESSAGE(clientManager->clientInfo(client1));
        BOOST_CHECK_EQUAL(clientManager->getClient([](const ClientSPtr &repoClient) { return repoClient->getPersonalID() == "23543234"; }, DebtorsRepo), client1);
        BOOST_CHECK_EQUAL(clientManager->getClient([](const ClientSPtr &repoClient) { return repoClient->getPersonalID() == "3"; }, DebtorsRepo), nullptr);

        BOOST_TEST_MESSAGE("End of ClientManagerFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(AccountManagerFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of AccountManagerFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        SavingsAccountSPtr account1(new SavingsAccount(AmountUPtr(new Amount(50L, 0, PLN))));
        SavingsAccountSPtr account2(new SavingsAccount(AmountUPtr(new Amount(50L, 0, PLN))));
        SavingsAccountSPtr account3(new SavingsAccount(AmountUPtr(new Amount(50L, 0, GBP))));

        AccountRepositorySPtr closedAccounts(new AccountRepository());
        AccountRepositorySPtr jointAccounts(new AccountRepository());
        AccountRepositorySPtr savingsAccounts(new AccountRepository());
        AccountRepositorySPtr currencyAccounts(new AccountRepository());

        AccountManagerUPtr accountManager(new AccountManager(closedAccounts, jointAccounts, savingsAccounts, currencyAccounts));

        BOOST_CHECK_THROW(AccountManager(nullptr, jointAccounts, savingsAccounts, currencyAccounts), AccountManagerConstructionException);
        BOOST_CHECK_THROW(AccountManager(closedAccounts, nullptr, savingsAccounts, currencyAccounts), AccountManagerConstructionException);
        BOOST_CHECK_THROW(AccountManager(closedAccounts, jointAccounts, nullptr, currencyAccounts), AccountManagerConstructionException);
        BOOST_CHECK_THROW(AccountManager(closedAccounts, jointAccounts, savingsAccounts, nullptr), AccountManagerConstructionException);

        BOOST_CHECK_THROW(accountManager->closeAccount(nullptr), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->closeAccount(account1), AccountManagerMethodException);

        AccountBuilderSPtr accountBuilder1(new AccountBuilder());
        AccountBuilderSPtr accountBuilder2(new AccountBuilder());

        vector<ClientSPtr> clients1{client1};

        accountBuilder1 = accountBuilder1
                ->setAmount(make_shared<Amount>(Amount(50L, 0, PLN)))
                ->setOwners(clients1)
                ->setAccountType(make_shared<AccountEnum>(SavingsAccountType));

        accountBuilder2 = accountBuilder2
                ->setAmount(make_shared<Amount>(Amount(50L, 0, PLN)))
                ->setOwners(clients1);

        BOOST_CHECK_THROW(accountManager->createAccount(nullptr), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->createAccount(accountBuilder2), AccountManagerException);
        BOOST_CHECK_NO_THROW(accountManager->createAccount(accountBuilder1));
        BOOST_CHECK(client1->getNumberOfAccounts() == 1);

        AccountSPtr account = accountManager->getAccount([](const AccountSPtr &acc) { return acc->getAmount()->getMainUnit() == 50L; }, SavingsAccountsRepo);

        BOOST_CHECK_NO_THROW(accountManager->addOwnerToAccount(account1, client1));
        BOOST_CHECK(client1->getNumberOfAccounts() == 2);
        BOOST_CHECK_NO_THROW(accountManager->closeAccount(account));
        BOOST_CHECK(client1->getNumberOfAccounts() == 1);
        BOOST_CHECK_EQUAL(*client1->getFounds()[PLN], Amount(50L, 0, PLN));
        BOOST_CHECK_THROW(accountManager->closeAccount(account), AccountManagerMethodException);
        BOOST_CHECK(*account->getAmount() == Amount(0L, 0, PLN));
        BOOST_CHECK(accountManager->getAccount([&client1](const AccountSPtr &acc) { return acc->getOwners()[0] == client1; }, ClosedAccountsRepo) != nullptr);

        BOOST_CHECK_THROW(accountManager->makeTransfer(nullptr, account2, AmountUPtr(new Amount(20L, 0, PLN))), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->makeTransfer(account1, nullptr, AmountUPtr(new Amount(20L, 0, PLN))), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->makeTransfer(account1, account2, nullptr), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->makeTransfer(account1, account2, AmountUPtr(new Amount(0L, 0, PLN))), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->makeTransfer(account1, account2, AmountUPtr(new Amount(20L, 0, GBP))), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->makeTransfer(account1, account3, AmountUPtr(new Amount(20L, 0, PLN))), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->makeTransfer(account1, account2, AmountUPtr(new Amount(120L, 0, PLN))), AccountManagerMethodException);
        BOOST_CHECK_NO_THROW(accountManager->makeTransfer(account1, account2, AmountUPtr(new Amount(50, 0, PLN))));
        BOOST_CHECK_EQUAL(*account2->getAmount(), Amount(100L, 0, PLN));
        BOOST_CHECK_EQUAL(*account1->getAmount(), Amount(0L, 0, PLN));
        BOOST_CHECK_EQUAL(*client1->getFounds()[PLN], Amount(45L, 0, PLN));

        BOOST_CHECK_THROW(accountManager->addOwnerToAccount(nullptr, client1), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->addOwnerToAccount(account1, nullptr), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->addOwnerToAccount(nullptr, client1), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->addOwnerToAccount(account1, client1), AccountManagerMethodException);

        account2->addOwner(client1);

        BOOST_CHECK_THROW(accountManager->addOwnerToAccount(account2, client1), AccountManagerMethodException);

        BOOST_CHECK_THROW(accountManager->removeOwnerFromAccount(nullptr, client1), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->removeOwnerFromAccount(account1, nullptr), AccountManagerMethodException);
        BOOST_CHECK_THROW(accountManager->removeOwnerFromAccount(account2, client1), AccountManagerMethodException);

        client1->addAccount(account2);
        account2->removeOwner(client1);

        BOOST_CHECK_THROW(accountManager->removeOwnerFromAccount(account2, client1), AccountManagerMethodException);

        account2->addOwner(client1);

        BOOST_CHECK_NO_THROW(accountManager->removeOwnerFromAccount(account2, client1));

        BOOST_CHECK(!client1->removeAccount(account2));
        BOOST_CHECK(!account2->removeOwner(client1));

        BOOST_CHECK_THROW(accountManager->accountInfo(nullptr), AccountManagerMethodException);
        BOOST_TEST_MESSAGE(accountManager->accountInfo(account1));

        BOOST_TEST_MESSAGE("End of AccountManagerFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(CardManagerFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of CardManagerFunctionalityTest:");

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        CurrencyAccountSPtr account1(new CurrencyAccount(AmountUPtr(new Amount(50L, 0, PLN))));
        CreditCardSPtr card1(new CreditCard(client1, account1));
        DebitCardSPtr card2(new DebitCard(client1, account1));

        client1->addAccount(account1);
        account1->addOwner(client1);

        CardRepositorySPtr lostCards(new CardRepository());
        CardRepositorySPtr canceledCards(new CardRepository());
        CardRepositorySPtr creditCards(new CardRepository());
        CardRepositorySPtr debitCards(new CardRepository());

        CardManagerUPtr cardManager(new CardManager(lostCards, canceledCards, creditCards, debitCards));

        BOOST_CHECK_THROW(CardManager(nullptr, canceledCards, creditCards, debitCards), CardManagerConstructionException);
        BOOST_CHECK_THROW(CardManager(lostCards, nullptr, creditCards, debitCards), CardManagerConstructionException);
        BOOST_CHECK_THROW(CardManager(lostCards, canceledCards, nullptr, debitCards), CardManagerConstructionException);
        BOOST_CHECK_THROW(CardManager(lostCards, canceledCards, creditCards, nullptr), CardManagerConstructionException);

        BOOST_CHECK_THROW(cardManager->cardInfo(nullptr), CardManagerMethodException);

        CardBuilderSPtr builder1(new CardBuilder());
        CardBuilderSPtr builder2(new CardBuilder());

        builder1 = builder1
                ->setCardType(make_shared<CardEnum>(CreditCardType))
                ->setAccount(account1)
                ->setOwner(client1);

        builder2 = builder2
                ->setAccount(account1)
                ->setOwner(client1);

        BOOST_CHECK_THROW(cardManager->createCard(nullptr), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->createCard(builder2), CardManagerException);
        BOOST_CHECK_NO_THROW(cardManager->createCard(builder1));

        BOOST_CHECK(client1->getNumberOfCards() == 1);
        BOOST_CHECK(account1->getCards().size() == 1);
        BOOST_CHECK(creditCards->getAll().size() == 1);

        CardSPtr myCard = creditCards->getAll()[0];

        BOOST_CHECK(cardManager->getCard([&myCard](const CardSPtr &other) { return myCard->getUuid() == other->getUuid(); }, CreditCardsRepo) != nullptr);

        BOOST_CHECK_THROW(cardManager->moveCardToRepository(nullptr, LostCardsRepo), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->moveCardToRepository(card1, LostCardsRepo), CardManagerMethodException);
        BOOST_CHECK_NO_THROW(cardManager->moveCardToRepository(myCard, LostCardsRepo));

        BOOST_CHECK(client1->getNumberOfCards() == 0);
        BOOST_CHECK(account1->getCards().size() == 0);
        BOOST_CHECK(creditCards->getAll().size() == 0);
        BOOST_CHECK(lostCards->getAll().size() == 1);

        BOOST_CHECK_NO_THROW(cardManager->moveCardToRepository(myCard, CreditCardsRepo));

        BOOST_CHECK(client1->getNumberOfCards() == 1);
        BOOST_CHECK(account1->getCards().size() == 1);
        BOOST_CHECK(creditCards->getAll().size() == 1);
        BOOST_CHECK(lostCards->getAll().size() == 0);

        BOOST_CHECK_THROW(cardManager->withdrawMoney(myCard, nullptr), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->withdrawMoney(nullptr, make_shared<Amount>(Amount(5L, 0, PLN))),CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->withdrawMoney(myCard, make_shared<Amount>(Amount(20L, 0, USD))), CardManagerException);
        BOOST_CHECK_THROW(cardManager->withdrawMoney(myCard, make_shared<Amount>(Amount(-1, 0, PLN))), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->withdrawMoney(myCard, make_shared<Amount>(Amount(60L, 0, PLN))), CardManagerMethodException);
        BOOST_CHECK_NO_THROW(cardManager->withdrawMoney(myCard, make_shared<Amount>(Amount(30L, 0, PLN))));

        BOOST_CHECK_EQUAL(*client1->getFounds()[PLN], Amount(30L, 0, PLN));
        BOOST_CHECK_EQUAL(*account1->getAmount(), Amount(18L, 0, PLN));

        BOOST_CHECK_THROW(cardManager->makePayment(nullptr, make_shared<Amount>(Amount(10L, 0, PLN))), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->makePayment(myCard, nullptr), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->makePayment(myCard, make_shared<Amount>(Amount(10L, 0, USD))), CardManagerException);
        BOOST_CHECK_THROW(cardManager->makePayment(myCard, make_shared<Amount>(Amount(30L, 0, PLN))), CardManagerMethodException);
        BOOST_CHECK_NO_THROW(cardManager->makePayment(myCard, make_shared<Amount>(Amount(10L, 0, PLN))));

        BOOST_CHECK_THROW(cardManager->payBack(card2, make_shared<Amount>(Amount(10L, 0, PLN))), CardManagerMethodException);
        BOOST_CHECK_THROW(cardManager->payBack(card1, make_shared<Amount>(Amount(10L, 0, USD))), CardManagerException);
        BOOST_CHECK_NO_THROW(cardManager->payBack(card1, make_shared<Amount>(Amount(10L, 0, PLN))));

        BOOST_TEST_MESSAGE("End of CardManagerFunctionalityTest");
    }

    BOOST_AUTO_TEST_CASE(LoanManagerFunctionalityTest) {
        BOOST_TEST_MESSAGE("Beginning of LoanManagerFunctionalityTest:");

        LoanRepositorySPtr currentLoans(new LoanRepository());
        LoanRepositorySPtr finalizedLoans(new LoanRepository());

        LoanManagerUPtr loanManager(new LoanManager(currentLoans, finalizedLoans));

        BOOST_CHECK_THROW(LoanManager(nullptr, finalizedLoans), LoanManagerConstructionException);
        BOOST_CHECK_THROW(LoanManager(currentLoans, nullptr), LoanManagerConstructionException);

        boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("CET"));
        boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
        shared_ptr<boost::local_time::local_date_time> time1(new boost::local_time::local_date_time(time, zone));
        unique_ptr<boost::local_time::local_date_time> time2(new boost::local_time::local_date_time(time, zone));

        ClientSPtr client1(new Client("John", "Kowalski", "23543234", AddressUPtr(new Address("Piotrkowska", "1"))));
        LoanBuilderSPtr loanBuilder(new LoanBuilder());

        loanBuilder = loanBuilder->setBorrower(client1)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->setBorrowDate(time1)
                ->setBorrowedAmount(make_shared<Amount>(Amount(3000L, 0, PLN)));

        BOOST_CHECK_THROW(loanManager->createLoan(nullptr), LoanManagerMethodException);
        BOOST_CHECK_THROW(loanManager->createLoan(make_shared<LoanBuilder>(LoanBuilder())), LoanManagerException);
        BOOST_CHECK_THROW(loanManager->createLoan(nullptr), LoanManagerMethodException);
        BOOST_CHECK_THROW(loanManager->createLoan(loanBuilder), LoanManagerMethodException);

        client1->getClientType()->setBaseServiceTier(0);

        loanBuilder = loanBuilder->setBorrower(client1)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->setBorrowDate(time1)
                ->setBorrowedAmount(make_shared<Amount>(Amount(10L, 0, PLN)));

        BOOST_CHECK_THROW(loanManager->createLoan(loanBuilder), LoanManagerMethodException);

        client1->getClientType()->setBaseServiceTier(20);

        loanBuilder = loanBuilder->setBorrower(client1)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->setBorrowDate(time1)
                ->setBorrowedAmount(make_shared<Amount>(Amount(10L, 0, PLN)));

        BOOST_CHECK_NO_THROW(loanManager->createLoan(loanBuilder));
        BOOST_CHECK_EQUAL(client1->getCreditworthiness(), 10);
        BOOST_CHECK_EQUAL(client1->getNumberOfLoans(), 1);
        BOOST_CHECK_EQUAL(currentLoans->getAll().size(), 1);

        LoanSPtr loan = loanManager->getLoan([&client1](const LoanSPtr &currentLoan) { return currentLoan->getBorrower() == client1; }, CurrentLoansRepo);
        LoanSPtr loan2(new StandardLoan(move(time2), AmountUPtr(new Amount(40L, 0, PLN)), client1));

        BOOST_CHECK_THROW(loanManager->returnMoney(nullptr, make_shared<Amount>(Amount(10L, 0, PLN))), LoanManagerMethodException);
        BOOST_CHECK_THROW(loanManager->returnMoney(loan2, make_shared<Amount>(Amount(10L, 0, PLN))), LoanManagerMethodException);
        BOOST_CHECK_NO_THROW(loanManager->returnMoney(loan, make_shared<Amount>(Amount(12L, 0, PLN))));
        BOOST_CHECK_THROW(loanManager->returnMoney(loan, make_shared<Amount>(Amount(12L, 0, PLN))), LoanManagerMethodException);


        loanBuilder = loanBuilder->setBorrower(client1)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->setBorrowDate(time1)
                ->setBorrowedAmount(make_shared<Amount>(Amount(10L, 0, PLN)));

        BOOST_CHECK_NO_THROW(loanManager->createLoan(loanBuilder));
        BOOST_CHECK_EQUAL(client1->getCreditworthiness(), 22);
        BOOST_CHECK_EQUAL(client1->getNumberOfLoans(), 1);
        BOOST_CHECK_EQUAL(currentLoans->getAll().size(), 1);
        BOOST_CHECK_EQUAL(finalizedLoans->getAll().size(), 1);

        loanBuilder = loanBuilder->setBorrower(client1)
                ->setLoanType(make_shared<LoanEnum>(StandardLoanType))
                ->setBorrowDate(time1)
                ->setBorrowedAmount(make_shared<Amount>(Amount(30L, 0, PLN)));

        BOOST_CHECK_NO_THROW(loanManager->createLoan(loanBuilder));
        BOOST_CHECK_EQUAL(client1->getCreditworthiness(), 17);
        BOOST_CHECK(*client1->getFounds()[PLN] < Amount(0L, 0, PLN));

        BOOST_CHECK_THROW(loanManager->loanInfo(nullptr), LoanManagerMethodException);
        BOOST_TEST_MESSAGE(loanManager->loanInfo(loan));

        LoanSPtr loan3 = loanManager->getLoan([](const LoanSPtr &currentLoan) { return currentLoan->getBorrowedAmount()->getMainUnit() == 30L; }, CurrentLoansRepo);

        BOOST_CHECK_THROW(loanManager->markAsFinalized(nullptr), LoanManagerMethodException);
        BOOST_CHECK_THROW(loanManager->markAsFinalized(loan2), LoanManagerMethodException);
        BOOST_CHECK_THROW(loanManager->markAsFinalized(nullptr), LoanManagerMethodException);
        BOOST_CHECK_NO_THROW(loanManager->markAsFinalized(loan3));

        BOOST_CHECK_EQUAL(client1->getNumberOfLoans(), 1);
        BOOST_CHECK_EQUAL(currentLoans->getAll().size(), 1);

        BOOST_TEST_MESSAGE("End of LoanManagerFunctionalityTest");
    }

BOOST_AUTO_TEST_SUITE_END()