//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_ADDRESS_H
#define BANKKONTA_ADDRESS_H


#include <string>

class Address {
private:
    std::string street;
    std::string number;

public:
    Address(std::string street, std::string number);

    ~Address();

    const std::string &getStreet() const;

    const std::string &getNumber() const;

    std::string toString() const;

};

typedef std::shared_ptr<Address> AddressSPtr;
typedef std::unique_ptr<Address> AddressUPtr;

#endif //BANKKONTA_ADDRESS_H
