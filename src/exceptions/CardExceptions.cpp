//
// Created by student on 12.01.2020.
//

#include "exceptions/CardExceptions.h"

using namespace std;

CardException::CardException(const string &message) : logic_error(message) {}

CardConstructionException::CardConstructionException(const string &message) : CardException(message) {}

CardModificationException::CardModificationException(const string &message) : CardException(message) {}

CardMethodException::CardMethodException(const string &message) : CardException(message) {}

CardClassException::CardClassException(const string &message) : CardException(message) {}

CardClassConstructionException::CardClassConstructionException(const string &message) : CardClassException(message) {}

CardClassMethodException::CardClassMethodException(const string &message) : CardClassException(message) {}

CardRepositoryException::CardRepositoryException(const string &message) : CardException(message) {}

CardBuilderException::CardBuilderException(const string &message) : CardException(message) {}

CardBuilderModificationException::CardBuilderModificationException(const string &message) : CardBuilderException(message) {}

CardBuilderBuildException::CardBuilderBuildException(const string &message) : CardBuilderException(message) {}

CardManagerException::CardManagerException(const string &message) : CardException(message) {}

CardManagerConstructionException::CardManagerConstructionException(const string &message) : CardManagerException(message) {}

CardManagerMethodException::CardManagerMethodException(const string &message) : CardManagerException(message) {}