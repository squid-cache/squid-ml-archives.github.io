#include <functional>
#include <memory>

/* for testing without OpenSSL */
struct BIO {};
extern "C" { void BIO_vfree(BIO *bio) { delete bio; } };


/* for extracting result and argument types from a unary function type */
template <typename T>
struct function_traits;
template <typename ReturnType, typename ArgType>
struct function_traits<ReturnType(ArgType)>
{
    typedef ReturnType result_type;
    typedef ArgType arg_type;
};


/* a functor that calls a hard-coded unary function */
template <class FunType, FunType *fun>
struct HardFun {
    typename function_traits<FunType>::result_type
    operator()(typename function_traits<FunType>::arg_type arg) { fun(arg); }
};


/* usage example */

typedef HardFun<decltype(BIO_vfree), &BIO_vfree> BIO_Deleter;
typedef std::unique_ptr<BIO, BIO_Deleter> BIO_Pointer;

int main() {
    BIO_Pointer bio(new BIO);
    return 0;
}
