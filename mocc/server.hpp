#ifndef MOCC_SERV_HPP_
#define MOCC_SERV_HPP_

#include "notifier.hpp"
#include "observer.hpp"

namespace mocc {

template <typename T, typename... U>
class Client
    : public Observer<U...>,
      public Notifier<Observer<U...> *, T> {

  public:
    using Host = Observer<U...> *;
};

template <typename T, typename... U>
class Server : public Observer<Observer<U...> *, T> {
  public:
    using Host = Observer<U...> *;
};

} // namespace mocc

#endif
