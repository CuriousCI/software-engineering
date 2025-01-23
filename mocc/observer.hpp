#ifndef MOCC_OBSERVER_HPP_
#define MOCC_OBSERVER_HPP_

template <typename... T> class Observer {
  public:
    virtual void update(T...) = 0;
};

#endif
