//
// Created by student on 12.01.2020.
//

#ifndef BANKKONTA_CARDEXCEPTIONS_H
#define BANKKONTA_CARDEXCEPTIONS_H

#include <string>
#include <stdexcept>

//=============================== CARD ===============================//

class CardException : public std::logic_error {
public:
    explicit CardException(const std::string &message);
};

class CardConstructionException : public CardException {
public:
    explicit CardConstructionException(const std::string &message);
};

class CardModificationException : public CardException {
public:
    explicit CardModificationException(const std::string &message);
};

//============================= DERIVED ===============================//

class CardMethodException : public CardException {
public:
    explicit CardMethodException(const std::string &message);
};

//============================ CARD CLASS =============================//

class CardClassException : public CardException {
public:
    explicit CardClassException(const std::string &message);
};

class CardClassConstructionException : public CardClassException {
public:
    explicit CardClassConstructionException(const std::string &message);
};

class CardClassMethodException : public CardClassException {
public:
    explicit CardClassMethodException(const std::string &message);
};

//=========================== CARD REPOSITORY ===========================//

class CardRepositoryException : public CardException {
public:
    explicit CardRepositoryException(const std::string &message);
};

//============================ CARD BUILDER =============================//

class CardBuilderException : public CardException {
public:
    explicit CardBuilderException(const std::string &message);
};

class CardBuilderModificationException : public CardBuilderException {
public:
    explicit CardBuilderModificationException(const std::string &message);
};

class CardBuilderBuildException : public CardBuilderException {
public:
    explicit CardBuilderBuildException(const std::string &message);
};

//============================ CARD MANAGER =============================//

class CardManagerException : public CardException {
public:
    explicit CardManagerException(const std::string &message);
};

class CardManagerConstructionException : public CardManagerException {
public:
    explicit CardManagerConstructionException(const std::string &message);
};

class CardManagerMethodException : public CardManagerException {
public:
    explicit CardManagerMethodException(const std::string &message);
};

#endif //BANKKONTA_CARDEXCEPTIONS_H
