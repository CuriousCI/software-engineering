#pragma once

/* https://refactoring.guru/design-patterns/observer */
template <typename... T> class Observer {
  public:
    /* This method is called when the entity is notified.
     * It must be overriden when the Observer class is inherited.
     * */
    virtual void update(T...) = 0;
};
