#pragma once

/* STRONG_ALIAS(WeightKg, float)
 *
 * WeightKg person_weight = 5.5;
 * float weight = person_weight;
 *
 * WeightKg and "float" are different types (in the type system), but their
 * values can be used interchangeably.
 * */
template <typename T> class Alias {
    T value;

  public:
    Alias() {}

    /* A value of type T can be assigned to the alias. */
    Alias(T value) : value(value) {}

    /* The alias can be assigned to a variable of type T. */
    operator T() const { return value; }
};

/* A strong alias X to a type Y is a type X different from Y that can be used as
 * if it was an Y. */
#define STRONG_ALIAS(ALIAS, TYPE)                                              \
    class ALIAS : public Alias<TYPE> {                                         \
      public:                                                                  \
        using Alias<TYPE>::Alias;                                              \
    };
