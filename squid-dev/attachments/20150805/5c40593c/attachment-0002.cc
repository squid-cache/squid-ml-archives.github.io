#include <cstdint>
#include <iostream>

// a sample enum
enum class Numbers {
  enumBegin_,

  zero = enumBegin_,
  one,
  two,
  three,

  enumEnd_ // maximum plus one
};

// another sample enum
enum class Colors: std::uint64_t  {
  enumBegin_ = 0xFFFF0000ULL,

  black = enumBegin_,
  red,
  white,

  enumEnd_ // maximum plus one
};

// To support range loops, we need to implement two APIs:
// EnumRange: A range container API (begin() and end() methods);
// EnumIterator: An iterator API (various operators: ++, *, !=).

/* Enum iterator operators. */
template <typename Enum>
Enum operator*(Enum value) { return value; }
template <typename Enum>
Enum operator++(Enum &value) { 
	typedef typename std::underlying_type<Enum>::type IntegralValue;
    return value = static_cast<Enum>(static_cast<IntegralValue>(value)+1);
}
template <typename Enum>
bool operator !=(Enum v1, Enum v2) { return v1 != v2; }
// TODO: Add suffix++, equality, and decrement operators for for/while loops.

// A range container API for an Enum.
// This EnumRange implementation focuses on the whole enum.
template <typename Enum>
class WholeEnum {
public:
  typedef Enum Iterator;
  Iterator begin() const { return Iterator(Enum::enumBegin_); }
  Iterator end() const { return Iterator(Enum::enumEnd_); }
};


// a hack to make dumping enum values simpler
template <typename Enum>
void dump(std::ostream &os, Enum value) {
    typedef typename std::underlying_type<Enum>::type IntegralValue;
	os << std::hex << static_cast<IntegralValue>(value) << ' ';
}


// sample code
int main() {

  /* Report all Numbers. */

  // beautiful range-for
  for (Numbers i: WholeEnum<Numbers>())
    dump(std::cout, i);
  std::cout << std::endl;

  // ugly explicit-for
  for (auto i = WholeEnum<Numbers>().begin(); i != WholeEnum<Numbers>().end(); ++i)
    dump(std::cout, *i);
  std::cout << std::endl;

  // beautiful explicit-for
  for (auto i = Numbers::enumBegin_; i != Numbers::enumEnd_; ++i)
    dump(std::cout, i);
  std::cout << std::endl;


  /* And now report all Colors. */

  // beautiful range-for
  for (Colors i: WholeEnum<Colors>())
    dump(std::cout, i);
  std::cout << std::endl;

  // ugly explicit-for
  for (auto i = WholeEnum<Colors>().begin(); i != WholeEnum<Colors>().end(); ++i)
    dump(std::cout, *i);
  std::cout << std::endl;

  // beautiful explicit-for
  for (auto i = Colors::enumBegin_; i != Colors::enumEnd_; ++i)
    dump(std::cout, i);
  std::cout << std::endl;

  return 0;
}
