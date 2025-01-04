#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <unordered_set>

#include "../case/serv.hpp"
#include "data.hpp"

class PostgreSQL : public lib::Server<Query, Reply> {
    std::vector<std::unordered_set<size_t>> database;

  public:
    PostgreSQL();

    void update(Host, Query) override;
};

#endif
