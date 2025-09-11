// g++ -Wall -std=c++11 -DUSE_OPENSSL -c t-lockingp.cc

/*
  A shared pointer to a reference-counting Object with customizable
  absorption, locking, and unlocking functions.

  The Absorb() function imports a raw Object pointer. It is usually identical
  to Lock(), but libraries like OpenSSL pre-lock objects before they are fed
  to LockingPointer, necessitating this customization hook.

  The Lock() function increments Object's reference counter.

  The Unlock() function decrements Object's reference counter and destroys
  the object when the counter reaches zero.
*/
template <typename Object,
    class Lock,
    class Unlock,
    class Absorb = Lock>
class LockingPointer;


#if USE_OPENSSL

// fake declarations of OpenSSL functions for testing
extern void CRYPTO_add(int *, int, int);
typedef struct { int references; } X509;
extern int CRYPTO_LOCK_X509;
extern void X509_free(X509 *x);

template <typename Object, int &Lock>
class OpenSslLocker {
public:
	void operator()(Object *x) { CRYPTO_add(x->references, 1, Lock); }
};

template <typename Object, void (*Unlock)(Object*) >
class OpenSslUnlocker {
public:
	void operator()(Object *x) { Unlock(x); }
};

template <typename Object>
class OpenSslAbsorber {
public:
	void operator()(Object *x) { /* OpenSSL locks raw objects for us */ }
};


template <typename Object,
    int &lock, // 3rd CRYPTO_add() parameter
    void (*Unlock)(Object *x) > // usually called Object_free()
using OpenSslLockingPointer = LockingPointer<Object,
    OpenSslLocker<Object, lock>,
    OpenSslUnlocker<Object, Unlock>,
    OpenSslAbsorber<Object> >;

typedef OpenSslLockingPointer<X509, CRYPTO_LOCK_X509, X509_free> CertPointer;

#endif /* USE_OPENSSL */
