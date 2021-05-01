//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CLIENTBUILDER_H
#define BANKKONTA_CLIENTBUILDER_H


#include <string>
#include <memory>
#include <vector>

class ClientType;

class Address;

class Loan;

class Amount;

class Card;

class Client;

class Account;


class ClientBuilder {
private:
    std::string firstName = "";
    std::string lastName = "";
    std::string personalID = "";
    short creditworthiness = 0;
    bool ownerOfCompany = false;
    std::shared_ptr<Address> registeredAddress = nullptr;
    std::shared_ptr<Address> address = nullptr;
    std::shared_ptr<Amount> income[4];
    std::shared_ptr<Amount> founds[4];
    std::shared_ptr<ClientType> clientType = nullptr;
    std::vector<std::shared_ptr<Account>> accounts;

public:
    ClientBuilder();

    ~ClientBuilder();

    std::shared_ptr<ClientBuilder> setFirstName(const std::string &_firstName);

    std::shared_ptr<ClientBuilder> setLastName(const std::string &_lastName);

    std::shared_ptr<ClientBuilder> setPersonalID(const std::string &_personalID);

    std::shared_ptr<ClientBuilder> setCreditworthiness(short _creditworthiness);

    std::shared_ptr<ClientBuilder> setOwnerOfCompany(bool _ownerOfCompany);

    std::shared_ptr<ClientBuilder> setRegisteredAddress(const std::shared_ptr<Address> &_registeredAddress);

    std::shared_ptr<ClientBuilder> setAddress(const std::shared_ptr<Address> &_address);

    std::shared_ptr<ClientBuilder> setClientType(const std::shared_ptr<ClientType> &_clientType);

    std::shared_ptr<ClientBuilder> setIncome(const std::shared_ptr<Amount> &_income);

    std::shared_ptr<ClientBuilder> setFounds(const std::shared_ptr<Amount> &_founds);

    std::shared_ptr<ClientBuilder> setAccounts(const std::vector<std::shared_ptr<Account>> &_accounts);

    std::shared_ptr<Client> build();

};

typedef std::shared_ptr<ClientBuilder> ClientBuilderSPtr;
typedef std::unique_ptr<ClientBuilder> ClientBuilderUPtr;

#endif //BANKKONTA_CLIENTBUILDER_H
