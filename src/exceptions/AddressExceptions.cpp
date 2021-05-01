//
// Created by student on 13.01.2020.
//

#include "exceptions/AddressExceptions.h"

using namespace std;

AddressException::AddressException(const string &message) : logic_error(message) {}