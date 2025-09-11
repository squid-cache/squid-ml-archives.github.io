#include <iostream>

// sample strongly-typed enum
enum class foo {
  zero,
  one,
  two,
  three
};

// generic iterator-over-enum
#include <type_traits>
template < typename C, C beginVal, C endVal>
class Iterator {
  typedef typename std::underlying_type<C>::type val_t;
  val_t val;
public:
  Iterator(const C & f) : val(static_cast<val_t>(f)) {}
  Iterator() : val(static_cast<val_t>(beginVal)) {}
  Iterator operator++() {
    ++val;
    return *this;
  }
  foo operator*() {
    return static_cast<C>(val);
  }
  Iterator begin() { return *this; }
  Iterator end() {
    static const Iterator endIter=++Iterator(endVal); //cache it
    return endIter;
  }
  bool operator!=(const Iterator& i) { return val != i.val; }
};

// specialize it
typedef Iterator<foo, foo::zero, foo::three> fooIterator;

// sample code
int main() {
  // range-for
  for (foo i: fooIterator()) {
    std::cout << static_cast<int>(i) << ','; // need to cast, it's not automatic
  }
  std::cout << std::endl;
  // explicit-for
  for (auto i = fooIterator().begin(); i != fooIterator(foo::two); ++i) {
    std::cout << static_cast<int>(*i) << ',';
  }
  std::cout << std::endl;
  return 0;

}
