//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CLIENT_H
#define BANKKONTA_CLIENT_H


#include <string>
#include <memory>
#include <vector>

class Address;

class Card;

class Amount;

class ClientType;

class Account;

class Loan;

class Client {
private:
    std::string firstName;
    std::string lastName;
    std::string personalID;
    short creditworthiness;
    bool ownerOfCompany;
    std::unique_ptr<Address> registeredAddress;
    std::unique_ptr<Address> address;
    std::vector<std::unique_ptr<Amount>> income;
    std::vector<std::unique_ptr<Amount>> founds;
    std::shared_ptr<ClientType> clientType;
    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<Card>> cards;
    std::vector<std::shared_ptr<Loan>> loans;

public:
    Client(std::string firstName, std::string lastName, std::string personalID, std::unique_ptr<Address> registeredAddress);

    ~Client();

    const std::string &getFirstName() const;

    void setFirstName(const std::string &_firstName);

    const std::string &getLastName() const;

    void setLastName(const std::string &_lastName);

    const std::string &getPersonalID() const;

    short getCreditworthiness() const;

    void setCreditworthiness(short _creditworthiness);

    bool isOwnerOfCompany() const;

    void setOwnerOfCompany(bool _ownerOfCompany);

    const std::unique_ptr<Address> &getRegisteredAddress() const;

    void setRegisteredAddress(std::unique_ptr<Address> _registeredAddress);

    const std::unique_ptr<Address> &getAddress() const;

    void setAddress(std::unique_ptr<Address> _address);

    const std::vector<std::unique_ptr<Amount>> &getIncome() const;

    void setIncome(std::unique_ptr<Amount> _income);

    const std::vector<std::unique_ptr<Amount>> &getFounds() const;

    void setFounds(std::unique_ptr<Amount> _founds);

    const std::shared_ptr<ClientType> &getClientType() const;

    void setClientType(const std::shared_ptr<ClientType> &_clientType);

    short getNumberOfCards() const;

    bool addCard(const std::shared_ptr<Card> &card);

    bool removeCard(const std::shared_ptr<Card> &card);

    short getNumberOfAccounts() const;

    bool addAccount(const std::shared_ptr<Account> &account);

    bool removeAccount(const std::shared_ptr<Account> &account);

    short getNumberOfLoans() const;

    bool addLoan(const std::shared_ptr<Loan> &loan);

    bool removeLoan(const std::shared_ptr<Loan> &loan);

    std::string briefClientInfo() const;

    std::string toString() const;

};

typedef std::shared_ptr<Client> ClientSPtr;
typedef std::unique_ptr<Client> ClientUPtr;

#endif //BANKKONTA_CLIENT_H
