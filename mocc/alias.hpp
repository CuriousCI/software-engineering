#pragma once

template <typename T> class Alias {
    T value;

  public:
    Alias() {}
    Alias(T value) : value(value) {}

    operator T() const { return value; }
};

#define STRONG_ALIAS(ALIAS, TYPE)                             \
    class ALIAS : public Alias<TYPE> {                        \
      public:                                                 \
        using Alias<TYPE>::Alias;                             \
    };
