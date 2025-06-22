#pragma once

#include <vector>

#include "observer.hpp"

/* https://refactoring.guru/design-patterns/observer */
/* A notifier is an entity that sends notifications of type T. */
/* It can support multiple types (e.g. Notifier<int, double>). */
template <typename... T> class Notifier {
  protected:
    std::vector<Observer<T...> *> observers;

  public:
    /* Adds an Observer to the Notifier. */
    void addObserver(Observer<T...> *observer) {
        observers.push_back(observer);
    }

    /* Notifies all the Observer of the Notifier. */
    virtual void notify(T... args) {
        for (auto observer : observers)
            observer->update(args...);
    }
};
