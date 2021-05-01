//
// Created by student on 12.01.2020.
//

#include "exceptions/AccountExceptions.h"

using namespace std;

AccountException::AccountException(const string &message) : logic_error(message) {}

AccountConstructionException::AccountConstructionException(const string &message) : AccountException(message) {}

AccountModificationException::AccountModificationException(const string &message) : AccountException(message) {}

AccountMethodException::AccountMethodException(const string &message) : AccountException(message) {}

JointAccountException::JointAccountException(const string &message) : AccountException(message) {}

JointAccountConstructionException::JointAccountConstructionException(const string &message) : JointAccountException(message) {}

JointAccountMethodException::JointAccountMethodException(const string &message) : JointAccountException(message) {}

AccountRepositoryException::AccountRepositoryException(const string &message) : AccountException(message) {}

AccountBuilderException::AccountBuilderException(const string &message) : AccountException(message) {}

AccountBuilderModificationException::AccountBuilderModificationException(const string &message) : AccountBuilderException(message) {}

AccountBuilderBuildException::AccountBuilderBuildException(const string &message) : AccountBuilderException(message) {}

AccountManagerException::AccountManagerException(const string &message) : AccountException(message) {}

AccountManagerConstructionException::AccountManagerConstructionException(const string &message) : AccountManagerException(message) {}

AccountManagerMethodException::AccountManagerMethodException(const string &message) : AccountManagerException(message) {}