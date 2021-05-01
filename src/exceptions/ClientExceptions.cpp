//
// Created by student on 12.01.2020.
//

#include "../../include/exceptions/ClientExceptions.h"

using namespace std;

ClientException::ClientException(const string &message) : logic_error(message) {}

ClientConstructionException::ClientConstructionException(const string &message) : ClientException(message) {}

ClientModificationException::ClientModificationException(const string &message) : ClientException(message) {}

ClientTypeException::ClientTypeException(const string &message) : ClientException(message) {}

ClientTypeConstructionException::ClientTypeConstructionException(const string &message) : ClientTypeException(message) {}

ClientTypeModificationException::ClientTypeModificationException(const string &message) : ClientTypeException(message) {}

ClientTypeMethodException::ClientTypeMethodException(const string &message) : ClientTypeException(message) {}

ClientRepositoryException::ClientRepositoryException(const string &message) : ClientException(message) {}

ClientBuilderException::ClientBuilderException(const string &message) : ClientException(message) {}

ClientBuilderModificationException::ClientBuilderModificationException(const string &message) : ClientBuilderException(message) {}

ClientBuilderBuildException::ClientBuilderBuildException(const string &message) : ClientBuilderException(message) {}

ClientManagerException::ClientManagerException(const string &message) : ClientException(message) {}

ClientManagerConstructionException::ClientManagerConstructionException(const string &message) : ClientManagerException(message) {}

ClientManagerMethodException::ClientManagerMethodException(const string &message) : ClientManagerException(message) {}
