#ifndef MOCC_ALIAS_HPP_
#define MOCC_ALIAS_HPP_

template <typename T>
class Alias {
    T value;

  public:
    Alias() {}
    Alias(T value) : value(value) {}

    operator T() const {
        return value;
    }
};

#define ALIAS_TYPE(ALIAS, TYPE)        \
    class ALIAS : public Alias<TYPE> { \
      public:                          \
        using Alias<TYPE>::Alias;      \
    };

#endif
