//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CLIENTREPOSITORY_H
#define BANKKONTA_CLIENTREPOSITORY_H


#include "Repository.h"
#include "enum/ClientTypesEnum.h"

class Client;

class Amount;

class ClientRepository : public Repository<Client> {
private:
    std::vector<std::shared_ptr<Client>> clients;

public:
    ClientRepository();

    ~ClientRepository() override;

    void add(const std::shared_ptr<Client> &element) override;

    void remove(const std::shared_ptr<Client> &element) override;

    std::vector<std::shared_ptr<Client>> getAll() override;

    std::shared_ptr<Client> find(const std::function<bool(std::shared_ptr<Client> client)> &predicate) override;

    void changeClientType(const std::shared_ptr<Client> &client, ClientTypesEnum clientType, float serviceCostMultiplier, short serviceTier, std::vector<std::unique_ptr<Amount>> maximumAmount);

};

typedef std::shared_ptr<ClientRepository> ClientRepositorySPtr;
typedef std::unique_ptr<ClientRepository> ClientRepositoryUPtr;

#endif //BANKKONTA_CLIENTREPOSITORY_H
