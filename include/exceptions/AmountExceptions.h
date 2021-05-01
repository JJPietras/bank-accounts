//
// Created by student on 13.01.2020.
//

#ifndef BANKKONTA_AMOUNTEXCEPTIONS_H
#define BANKKONTA_AMOUNTEXCEPTIONS_H

#include <string>
#include <stdexcept>

class AmountException : public std::logic_error {
public:
    explicit AmountException(const std::string &message);
};

class AmountConstructionException : public AmountException {
public:
    explicit AmountConstructionException(const std::string &message);
};

class AmountOperatorException : public AmountException {
public:
    explicit AmountOperatorException(const std::string &message);
};

#endif //BANKKONTA_AMOUNTEXCEPTIONS_H
