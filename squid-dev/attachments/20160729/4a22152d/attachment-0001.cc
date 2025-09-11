#include <functional>
#include <memory>

/* for testing without OpenSSL */
struct BIO {};
extern "C" { void BIO_vfree(BIO *bio) { delete bio; } };


/* a functor that calls a hard-coded unary function */
template <class ReturnType, class ArgType, ReturnType (*fun)(ArgType)>
struct HardFun {
    ReturnType operator()(ArgType arg) { fun(arg); }
};

/* usage example */

typedef HardFun<void, BIO*, &BIO_vfree> BIO_Deleter;
typedef std::unique_ptr<BIO, BIO_Deleter> BIO_Pointer;

int main() {
    BIO_Pointer bio(new BIO);
    return 0;
}
