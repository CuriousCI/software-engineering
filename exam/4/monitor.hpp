#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../case/data.hpp"
#include "../case/stat.hpp"
#include "../case/subscriber.hpp"
#include "../case/util.hpp"
#include "data.hpp"

class Monitor :
    public lib::Data<Time>,
    public Subscriber<HttpResponse>
{
   public:
    const size_t id;
    Stat response_time;

    Monitor(const size_t id);

    void update(HttpResponse) override;
};

#endif
