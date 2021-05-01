//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CARDREPOSITORY_H
#define BANKKONTA_CARDREPOSITORY_H


#include "Repository.h"
#include "enum/CardTypesEnum.h"

class Card;

class CardRepository : public Repository<Card> {
private:
    std::vector<std::shared_ptr<Card>> cards;

public:
    CardRepository();

    ~CardRepository() override;

    void add(const std::shared_ptr<Card> &element) override;

    void remove(const std::shared_ptr<Card> &element) override;

    std::vector<std::shared_ptr<Card>> getAll() override;

    std::shared_ptr<Card> find(const std::function<bool(std::shared_ptr<Card> card)> &predicate) override;

};

typedef std::shared_ptr<CardRepository> CardRepositorySPtr;
typedef std::unique_ptr<CardRepository> CardRepositoryUPtr;

#endif //BANKKONTA_CARDREPOSITORY_H
