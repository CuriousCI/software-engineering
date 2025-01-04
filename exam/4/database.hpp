#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <unordered_set>

#include "../case/serv.hpp"
#include "data.hpp"

class PostgreSQL : public lib::Server<Query, Reply>
{
    const size_t K;
    std::vector<std::unordered_set<size_t>> database;

   public:
    PostgreSQL(const size_t F, const size_t K);

    void update(Host, Query) override;
};

#endif
