#pragma once

#include "notifier.hpp"
#include "observer.hpp"

/* The namespace is needed because it often happens in exercises to create a
 * custom "Server" class, so it's useful to solve name conflicts.
 * */
namespace mocc {

/* A client receives responses from a server, and sends requests.
 * A request is the pair (Observer<U...> *, T), where 'Observer<U...> *' is the
 * Client itself, and T is the value of the request.
 * */
template <typename T, typename... U>
class Client : public Observer<U...>, public Notifier<Observer<U...> *, T> {
  public:
    using Host = Observer<U...> *;
};

/* A server receives requests from the clients.
 * A request is the pair (Observer<U...> *, T), where 'Observer<U...> *' is the
 * Client itself, and T is the value of the request.
 * */
template <typename T, typename... U>
class Server : public Observer<Observer<U...> *, T> {
  public:
    using Host = Observer<U...> *;
};

} // namespace mocc
