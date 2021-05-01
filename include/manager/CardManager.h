//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CARDMANAGER_H
#define BANKKONTA_CARDMANAGER_H


#include <vector>
#include <memory>
#include <functional>

#include "enum/CardRepositoriesEnum.h"

class Amount;

class Card;

class CardBuilder;

class CardRepository;

class CardManager {
private:
    std::shared_ptr<CardRepository> lostCards;
    std::shared_ptr<CardRepository> canceledCards;
    std::shared_ptr<CardRepository> creditCards;
    std::shared_ptr<CardRepository> debitCards;

    std::shared_ptr<CardRepository> getRepository(CardRepositoriesEnum repository);

public:
    CardManager(std::shared_ptr<CardRepository> lostCards, std::shared_ptr<CardRepository> canceledCards, std::shared_ptr<CardRepository> creditCards, std::shared_ptr<CardRepository> debitCards);

    ~CardManager();

    void createCard(const std::shared_ptr <CardBuilder> &prePreparedCard);

    std::shared_ptr<Card> getCard(const std::function<bool(std::shared_ptr<Card>)> &predicate, CardRepositoriesEnum repository);

    void moveCardToRepository(const std::shared_ptr<Card> &card, CardRepositoriesEnum repository);

    void withdrawMoney(const std::shared_ptr<Card> &card, const std::shared_ptr<Amount> &amount);

    void makePayment(const std::shared_ptr<Card> &card, const std::shared_ptr<Amount> &amount);

    void payBack(const std::shared_ptr<Card> &card, const std::shared_ptr<Amount> &amount);

    std::string cardInfo(const std::shared_ptr<Card> &card) const;

};

typedef std::shared_ptr<CardManager> CardManagerSPtr;
typedef std::unique_ptr<CardManager> CardManagerUPtr;

#endif //BANKKONTA_CARDMANAGER_H
