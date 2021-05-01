//
// Created by student on 13.01.2020.
//

#ifndef BANKKONTA_ADDRESSEXCEPTIONS_H
#define BANKKONTA_ADDRESSEXCEPTIONS_H

#include <string>
#include <stdexcept>

class AddressException : public std::logic_error {
public:
    explicit AddressException(const std::string &message);
};


#endif //BANKKONTA_ADDRESSEXCEPTIONS_H
