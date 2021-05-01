//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_CLIENTMANAGER_H
#define BANKKONTA_CLIENTMANAGER_H


#include <memory>
#include <functional>

#include "enum/ClientRepositoriesEnum.h"

class Client;

class ClientRepository;

class ClientManager {
private:
    std::shared_ptr<ClientRepository> formerClients;
    std::shared_ptr<ClientRepository> debtors;
    std::shared_ptr<ClientRepository> standardClients;

    std::shared_ptr<ClientRepository> getRepository(ClientRepositoriesEnum repository) const;

public:
    ClientManager(std::shared_ptr<ClientRepository> formerClients, std::shared_ptr<ClientRepository> debtors, std::shared_ptr<ClientRepository> standardClients);

    ~ClientManager();

    void addClient(const std::shared_ptr<Client> &client, ClientRepositoriesEnum repository);

    std::shared_ptr<Client> getClient(const std::function<bool(std::shared_ptr<Client> client)> &predicate, ClientRepositoriesEnum repository);

    void changeClientStatus(const std::shared_ptr<Client> &client, ClientRepositoriesEnum targetRepository);

    std::string clientInfo(const std::shared_ptr<Client> &client) const;

};

typedef std::shared_ptr<ClientManager> ClientManagerSPtr;
typedef std::unique_ptr<ClientManager> ClientManagerUPtr;

#endif //BANKKONTA_CLIENTMANAGER_H
