//
// Created by student on 12.01.2020.
//

#ifndef BANKKONTA_CLIENTEXCEPTIONS_H
#define BANKKONTA_CLIENTEXCEPTIONS_H

#include <string>
#include <stdexcept>

//=============================== CLIENT ===============================//

class ClientException : public std::logic_error {
public:
    explicit ClientException(const std::string &message);
};

class ClientConstructionException : public ClientException {
public:
    explicit ClientConstructionException(const std::string &message);
};

class ClientModificationException : public ClientException {
public:
    explicit ClientModificationException(const std::string &message);
};

//============================= CLIENT TYPE =============================//

class ClientTypeException : public ClientException {
public:
    explicit ClientTypeException(const std::string &message);
};

class ClientTypeConstructionException : public ClientTypeException {
public:
    explicit ClientTypeConstructionException(const std::string &message);
};

class ClientTypeModificationException : public ClientTypeException {
public:
    explicit ClientTypeModificationException(const std::string &message);
};

class ClientTypeMethodException : public ClientTypeException {
public:
    explicit ClientTypeMethodException(const std::string &message);
};

//========================== CLIENT REPOSITORY ==========================//

class ClientRepositoryException : public ClientException {
public:
    explicit ClientRepositoryException(const std::string &message);
};

//=========================== CLIENT BUILDER ============================//

class ClientBuilderException : public ClientException {
public:
    explicit ClientBuilderException(const std::string &message);
};

class ClientBuilderModificationException : public ClientBuilderException {
public:
    explicit ClientBuilderModificationException(const std::string &message);
};

class ClientBuilderBuildException : public ClientBuilderException {
public:
    explicit ClientBuilderBuildException(const std::string &message);
};

//=========================== CLIENT MANAGER ============================//

class ClientManagerException : public ClientException {
public:
    explicit ClientManagerException(const std::string &message);
};

class ClientManagerConstructionException : public ClientManagerException {
public:
    explicit ClientManagerConstructionException(const std::string &message);
};

class ClientManagerMethodException : public ClientManagerException {
public:
    explicit ClientManagerMethodException(const std::string &message);
};

#endif //BANKKONTA_CLIENTEXCEPTIONS_H
