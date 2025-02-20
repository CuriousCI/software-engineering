#pragma once

#include <vector>

#include "observer.hpp"

template <typename... T> class Notifier {
  protected:
    std::vector<Observer<T...> *> observers;

  public:
    void addObserver(Observer<T...> *observer) {
        observers.push_back(observer);
    }

    virtual void notify(T... args) {
        for (auto observer : observers)
            observer->update(args...);
    }
};
