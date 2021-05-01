//
// Created by student on 21.12.2019.
//

#include <algorithm>
#include <memory>

#include "repository/ClientRepository.h"
#include "model/Client.h"
#include "model/Amount.h"
#include "model/NewClient.h"
#include "model/NormalClient.h"
#include "model/TrustworthyClient.h"
#include "model/CommonDebtor.h"
#include "exceptions/ClientExceptions.h"
#include "../exceptions/ExceptionsMessages.cpp"

using namespace std;

ClientRepository::ClientRepository() = default;

ClientRepository::~ClientRepository() = default;

void ClientRepository::add(const ClientSPtr &element) {
    if (element == nullptr) {
        throw ClientRepositoryException(NULL_CLIENT);
    }
    if (clients.empty()) {
        clients.emplace_back(element);
        return;
    }
    auto possibleExistingClient = find([&element](const ClientSPtr &client) { return element->getPersonalID() == client->getPersonalID(); });
    if (possibleExistingClient == nullptr) {
        clients.emplace_back(element);
    } else {
        throw ClientRepositoryException(CLIENT_EXISTS);
    }
}

void ClientRepository::remove(const ClientSPtr &element) {
    if (element == nullptr) {
        throw ClientRepositoryException(NULL_CLIENT);
    }
    if (clients.empty()) {
        throw ClientRepositoryException(EMPTY_COLLECTION);
    }

    auto clientIterator = clients.begin();
    for (; clientIterator != clients.end(); clientIterator++) {
        if ((*clientIterator)->getPersonalID() == element->getPersonalID()) {
            clients.erase(clientIterator);
            return;
        }
    }
    throw ClientRepositoryException(CLIENT_EXISTS);
}

vector<ClientSPtr> ClientRepository::getAll() {
    return clients;
}

ClientSPtr ClientRepository::find(const function<bool(ClientSPtr client)> &predicate) {
    auto found = find_if(clients.begin(), clients.end(), predicate);
    if (found == clients.end()) {
        return nullptr;
    }
    return *found;
}

void ClientRepository::changeClientType(const ClientSPtr &client, ClientTypesEnum clientType, float serviceCostMultiplier, short serviceTier, vector<AmountUPtr> maximumAmount) {
    if (client == nullptr) {
        throw ClientRepositoryException(NULL_CLIENT);
    }
    switch (clientType) {
        case NewClientType:
            client->setClientType(make_shared<NewClient>(serviceCostMultiplier, serviceTier, move(maximumAmount)));
            break;
        case NormalClientType:
            client->setClientType(make_shared<NormalClient>(serviceCostMultiplier, serviceTier, move(maximumAmount)));
            break;
        case CommonDebtorType:
            client->setClientType(make_shared<CommonDebtor>(serviceCostMultiplier, serviceTier, move(maximumAmount)));
            break;
        case TrustworthyClientType:
            client->setClientType(make_shared<TrustworthyClient>(serviceCostMultiplier, serviceTier, move(maximumAmount)));
            break;
    }
}
