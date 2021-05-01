//
// Created by student on 21.12.2019.
//


#include <utility>
#include <stdexcept>
#include <boost/algorithm/string.hpp>

#include "model/Address.h"
#include "exceptions/AddressExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

Address::Address(string street, string number) : street(move(street)), number(move(number)) {
    boost::trim(this->street);
    boost::trim(this->number);
    if (this->street.empty() || this->number.empty()) {
        throw AddressException(EMPTY_ADDRESS);
    }
}

Address::~Address() = default;

const string &Address::getStreet() const {
    return street;
}

const string &Address::getNumber() const {
    return number;
}

string Address::toString() const {
    return street + " " + number;
}