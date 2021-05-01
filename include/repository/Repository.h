//
// Created by student on 21.12.2019.
//

#ifndef BANKKONTA_REPOSITORY_H
#define BANKKONTA_REPOSITORY_H


#include <vector>
#include <memory>
#include <functional>

template<class T>
class Repository {
public:
    virtual ~Repository() = default;

    virtual void add(const std::shared_ptr<T> &element) = 0;

    virtual void remove(const std::shared_ptr<T> &element) = 0;

    virtual std::vector<std::shared_ptr<T>> getAll() = 0;

    virtual std::shared_ptr<T> find(const std::function<bool(std::shared_ptr<T> element)> &predicate) = 0;

};

typedef std::shared_ptr<Repository<class T>> RepositorySPtr;
typedef std::unique_ptr<Repository<class T>> RepositoryUPtr;

#endif //BANKKONTA_REPOSITORY_H
