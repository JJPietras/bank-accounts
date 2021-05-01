//
// Created by student on 21.12.2019.
//

#include <utility>

#include "manager/CardManager.h"
#include "repository/CardRepository.h"
#include "builder/CardBuilder.h"
#include "model/DebitCard.h"
#include "model/CreditCard.h"
#include "model/Account.h"
#include "model/Amount.h"
#include "model/Client.h"
#include "model/Card.h"
#include "enum/CardRepositoriesEnum.h"
#include "exceptions/AmountExceptions.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

CardManager::CardManager(CardRepositorySPtr lostCards, CardRepositorySPtr canceledCards, CardRepositorySPtr creditCards, CardRepositorySPtr debitCards)
        : lostCards(move(lostCards)), canceledCards(move(canceledCards)), creditCards(move(creditCards)), debitCards(move(debitCards)) {
    if (this->lostCards == nullptr || this->canceledCards == nullptr || this->creditCards == nullptr || this->debitCards == nullptr) {
        throw CardManagerConstructionException(NULL_REPO);
    }
}

CardManager::~CardManager() = default;

void CardManager::createCard(const CardBuilderSPtr &prePreparedCard) {
    if (prePreparedCard == nullptr) {
        throw CardManagerMethodException(NULL_BUILDER);
    }
    CardSPtr card;
    try {
        card = prePreparedCard->build();
    }
    catch (CardException &) {
        throw CardManagerException(BUILDER_EXCEPTION);
    }

    card->getAccount()->addCard(card);
    card->getOwner()->addCard(card);

    if (dynamic_cast<DebitCard *>(card.get()) != nullptr) {
        debitCards->add(card);
    } else {
        creditCards->add(card);
    }
}

CardSPtr CardManager::getCard(const function<bool(CardSPtr)> &predicate, CardRepositoriesEnum repository) {
    return getRepository(repository)->find(predicate);
}

void CardManager::moveCardToRepository(const CardSPtr &card, CardRepositoriesEnum repository) {
    if (card == nullptr) {
        throw CardManagerMethodException(NULL_CARD);
    }

    CardRepositorySPtr repos[]{lostCards, canceledCards, debitCards, creditCards};
    bool removed = false;
    int i = 0;
    for (; i < 4; i++) {
        if (repos[i]->find([&card](const CardSPtr &otherCard) { return card->getUuid() == otherCard->getUuid(); }) != nullptr) {
            repos[i]->remove(card);
            removed = true;
            break;
        }
    }
    if (!removed) {
        throw CardManagerMethodException(ELEMENT_NOT_REMOVED);
    }
    getRepository(repository)->add(card);
    if (repository == LostCardsRepo || repository == CanceledCardsRepo) {
        card->getOwner()->removeCard(card);
        card->getAccount()->removeCard(card);
    } else if (i < 1 && (repository == DebitCardsRepo || repository == CreditCardsRepo)) {
        card->getAccount()->addCard(card);
        card->getOwner()->addCard(card);
    }
}

void CardManager::withdrawMoney(const CardSPtr &card, const AmountSPtr &amount) {
    if (card == nullptr) {
        throw CardManagerMethodException(NULL_CARD);
    }
    if (amount == nullptr) {
        throw CardManagerMethodException(NULL_AMOUNT);
    }
    if (*amount < Amount(0L, 0, amount->getCurrency())) {
        throw CardManagerMethodException(NEGATIVE_AMOUNT);
    }

    AmountSPtr withdrawCost = card->calculateWithdrawCost();
    AmountSPtr totalCost;
    try {
        totalCost = make_shared<Amount>(*amount + *withdrawCost);
    }
    catch (AmountOperatorException &) {
        throw CardManagerException(MULTIPLE_CURRENCIES);
    }

    if (*card->getAmount() > *totalCost || *card->getAmount() == *totalCost) {
        card->getAccount()->setAmount(AmountUPtr(new Amount(*card->getAmount() - *totalCost)));
        card->getOwner()->setFounds(AmountUPtr(new Amount(*card->getOwner()->getFounds()[amount->getCurrency()] + *amount)));
    } else {
        throw CardManagerMethodException(TOO_EXPENSIVE);
    }
}

void CardManager::makePayment(const CardSPtr &card, const AmountSPtr &amount) {
    if (card == nullptr) {
        throw CardManagerMethodException(NULL_CARD);
    }
    if (amount == nullptr) {
        throw CardManagerMethodException(NULL_AMOUNT);
    }

    bool result;

    try {
        result = card->makePayment(AmountUPtr(new Amount(*amount)));
    }
    catch (CardMethodException &) {
        throw CardManagerException(WRONG_AMOUNT);
    }
    if (!result) {
        throw CardManagerMethodException(TOO_EXPENSIVE);
    }
}

void CardManager::payBack(const CardSPtr &card, const AmountSPtr &amount) {
    if (dynamic_cast<CreditCard *>(card.get()) == nullptr) {
        throw CardManagerMethodException(NOT_CREDIT);
    }
    CreditCardSPtr credit = dynamic_pointer_cast<CreditCard>(card);
    try {
        credit->payBack(AmountUPtr(new Amount(*amount)));
    }
    catch (CardMethodException &) {
        throw CardManagerException(WRONG_AMOUNT);
    }
}

string CardManager::cardInfo(const CardSPtr &card) const {
    if (card == nullptr) {
        throw CardManagerMethodException(NULL_CARD);
    }
    return card->toString();
}

CardRepositorySPtr CardManager::getRepository(CardRepositoriesEnum repository) {
    CardRepositorySPtr repo;
    switch (repository) {
        case LostCardsRepo:
            repo = lostCards;
            break;
        case CanceledCardsRepo:
            repo = canceledCards;
            break;
        case CreditCardsRepo:
            repo = creditCards;
            break;
        case DebitCardsRepo:
            repo = debitCards;
            break;
    }
    return repo;
}
