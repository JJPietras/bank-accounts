//
// Created by student on 12.01.2020.
//

#ifndef BANKKONTA_ACCOUNTEXCEPTIONS_H
#define BANKKONTA_ACCOUNTEXCEPTIONS_H


#include <string>
#include <stdexcept>

//=============================== ACCOUNT ===============================//

class AccountException : public std::logic_error {
public:
    explicit AccountException(const std::string &message);
};

class AccountConstructionException : public AccountException {
public:
    explicit AccountConstructionException(const std::string &message);
};

class AccountModificationException : public AccountException {
public:
    explicit AccountModificationException(const std::string &message);
};

class AccountMethodException : public AccountException {
public:
    explicit AccountMethodException(const std::string &message);
};

//============================= DERIVED ===============================//

class JointAccountException : public AccountException {
public:
    explicit JointAccountException(const std::string &message);
};

class JointAccountConstructionException : public JointAccountException {
public:
    explicit JointAccountConstructionException(const std::string &message);
};

class JointAccountMethodException : public JointAccountException {
public:
    explicit JointAccountMethodException(const std::string &message);
};

//======================== ACCOUNT REPOSITORY =========================//

class AccountRepositoryException : public AccountException {
public:
    explicit AccountRepositoryException(const std::string &message);
};

//========================= ACCOUNT BUILDER ===========================//

class AccountBuilderException : public AccountException {
public:
    explicit AccountBuilderException(const std::string &message);
};

class AccountBuilderModificationException : public AccountBuilderException {
public:
    explicit AccountBuilderModificationException(const std::string &message);
};

class AccountBuilderBuildException : public AccountBuilderException {
public:
    explicit AccountBuilderBuildException(const std::string &message);
};

//========================= ACCOUNT MANAGER ===========================//

class AccountManagerException : public AccountException {
public:
    explicit AccountManagerException(const std::string &message);
};

class AccountManagerConstructionException : public AccountManagerException {
public:
    explicit AccountManagerConstructionException(const std::string &message);
};

class AccountManagerMethodException : public AccountManagerException {
public:
    explicit AccountManagerMethodException(const std::string &message);
};

#endif //BANKKONTA_ACCOUNTEXCEPTIONS_H
