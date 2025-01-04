#include "database.hpp"

PostgreSQL::PostgreSQL() : database(F) {};

void PostgreSQL::update(Host host, Query query) {
    bool is_confirmed = false;
    Data data = query;

    if (database[data.f - 1].size() < 20) {
        database[data.f - 1].insert(data.id);
        is_confirmed = true;
    } else if (database[data.f - 1].find(data.id) != database[data.f - 1].end())
        is_confirmed = true;

    host->update(Resp{is_confirmed, data.time});
}
