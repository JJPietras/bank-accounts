//
// Created by student on 21.12.2019.
//

#include <algorithm>

#include "repository/CardRepository.h"
#include "model/Card.h"
#include "exceptions/CardExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

CardRepository::CardRepository() = default;

CardRepository::~CardRepository() = default;

void CardRepository::add(const CardSPtr &element) {
    if (element == nullptr) {
        throw CardRepositoryException(NULL_CARD);
    }
    if (cards.empty()) {
        cards.emplace_back(element);
        return;
    }
    auto possibleExistingCard = find([&element](const CardSPtr &card) { return element->getUuid() == card->getUuid(); });
    if (possibleExistingCard == nullptr) {
        cards.emplace_back(element);
    } else {
        throw CardRepositoryException(CARD_EXISTS);
    }
}

void CardRepository::remove(const CardSPtr &element) {
    if (element == nullptr) {
        throw CardRepositoryException(NULL_CARD);
    }
    if (cards.empty()) {
        throw CardRepositoryException(EMPTY_COLLECTION);
    }

    auto cardIterator = cards.begin();
    for (; cardIterator != cards.end(); cardIterator++) {
        if ((*cardIterator)->getUuid() == element->getUuid()) {
            cards.erase(cardIterator);
            return;
        }
    }
    throw CardRepositoryException(CARD_EXISTS);
}

vector<CardSPtr> CardRepository::getAll() {
    return cards;
}

CardSPtr CardRepository::find(const function<bool(CardSPtr card)> &predicate) {
    auto found = find_if(cards.begin(), cards.end(), predicate);
    if (found == cards.end()) {
        return nullptr;
    }
    return *found;
}
