//
// Created by student on 13.01.2020.
//

#include "exceptions/AmountExceptions.h"

using namespace std;

AmountException::AmountException(const string &message) : logic_error(message) {}

AmountConstructionException::AmountConstructionException(const string &message) : AmountException(message) {}

AmountOperatorException::AmountOperatorException(const string &message) : AmountException(message) {}
