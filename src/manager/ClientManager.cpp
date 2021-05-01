//
// Created by student on 21.12.2019.
//

#include <utility>

#include "manager/ClientManager.h"
#include "repository/ClientRepository.h"
#include "model/Client.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

ClientManager::ClientManager(ClientRepositorySPtr formerClients, ClientRepositorySPtr debtors, ClientRepositorySPtr standardClients)
        : formerClients(move(formerClients)), debtors(move(debtors)), standardClients(move(standardClients)) {
    if (this->formerClients == nullptr || this->debtors == nullptr || this->standardClients == nullptr) {
        throw ClientManagerConstructionException(NULL_REPO);
    }
}

ClientManager::~ClientManager() = default;

void ClientManager::    addClient(const ClientSPtr &client, ClientRepositoriesEnum repository) {
    try {
        getRepository(repository)->add(client);
    }
    catch (logic_error &) {
        throw ClientManagerException(ELEMENT_IN_REPO);
    }
}

ClientSPtr ClientManager::getClient(const function<bool(ClientSPtr client)> &predicate, ClientRepositoriesEnum repository) {
    return getRepository(repository)->find(predicate);
}

void ClientManager::changeClientStatus(const ClientSPtr &client, ClientRepositoriesEnum targetRepository) {
    if (client == nullptr) {
        throw ClientManagerMethodException(NULL_CLIENT);
    }
    ClientRepositorySPtr repos[]{standardClients, formerClients, debtors};
    bool removed = false;
    for (const auto &r : repos) {
        if (r->find([&client](const ClientSPtr &otherClient) { return client->getPersonalID() == otherClient->getPersonalID(); }) != nullptr) {
            r->remove(client);
            removed = true;
            break;
        }
    }
    if (!removed) {
        throw ClientManagerMethodException(ELEMENT_NOT_REMOVED);
    }
    getRepository(targetRepository)->add(client);
}

string ClientManager::clientInfo(const ClientSPtr &client) const {
    if (client == nullptr) {
        throw ClientManagerMethodException(NULL_CLIENT);
    }
    return client->toString();
}

ClientRepositorySPtr ClientManager::getRepository(ClientRepositoriesEnum repository) const {
    ClientRepositorySPtr repo;
    switch (repository) {
        case StandardClientsRepo:
            repo = standardClients;
            break;
        case FormerClientsRepo:
            repo = formerClients;
            break;
        case DebtorsRepo:
            repo = debtors;
    }
    return repo;
}


