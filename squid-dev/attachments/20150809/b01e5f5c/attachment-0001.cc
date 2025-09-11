#include <iostream>
#include <string>
#include <stack>

#define SBuf std::string

// Assembles information for a single Cache Manager "page".
// Supplied information goes into a configured stream.
// Kids implement various information formatting methods.
class Page: public std::ostream {
public:
    Page(std::ostream &os);
    virtual ~Page() {}

    /// prepares a page for recording; called at most once per page
    virtual Page &open() = 0;
    /// prepares recording for a page; called once per opened page
    virtual Page &close() = 0;

    // a comment
    virtual Page &comment(const SBuf &text) = 0;

    /// starts recording key:value pairs
    virtual Page &beginHash() = 0;
    /// finishes recording key:value pairs
    virtual Page &endHash() = 0;

    /// starts recording a list of values
    virtual Page &beginList() = 0;
    /// finishes recording a list of values
    virtual Page &endList() = 0;

    /// starts recording a list of values
    virtual Page &beginListItem() = 0;
    /// finishes recording a list of values
    virtual Page &endListItem() = 0;

    /// starts recording a key:value pair with a given key
    virtual Page &beginKv(const SBuf &key) = 0;
    /// finishes recording a key:value pair
    virtual Page &endKv() = 0;

    // a single key:value pair with an atomic value
    Page &kv(const SBuf &key, const SBuf &atomicValue) {
        beginKv(key);
        atom(atomicValue);
        endKv();
        return *this;
    }

protected:
    // low-level method for writing a single token or "piece of data"
    virtual Page &atom(const SBuf &text) = 0;

private:
    typedef enum { ctxNone, ctxHash, ctxList, etc} Context;
    std::stack<Context> context; // what [values] are we expecting now?
};

// A Page formatted using YAML syntax.
// XXX: Does not really honor most YAML syntax rules yet, including indenting.
class YamlPage: public Page {
public:
    YamlPage(std::ostream &os): Page(os), nesting(0) {}
    virtual ~YamlPage() {}

    /* Page API */
    virtual Page &open() override { *this << "---" << std::endl; return *this; }
    virtual Page &close() override { *this << "---" << std::endl; return *this; }
    virtual Page &comment(const SBuf &text) { *this << "# " << text << std::endl; return *this; }
    virtual Page &beginHash() override { ++nesting; return *this; }
    virtual Page &endHash() override { --nesting; return *this; }
    virtual Page &beginList() override { ++nesting; return *this; }
    virtual Page &endList() override { --nesting; return *this; }
    virtual Page &beginListItem() override { *this << "- "; return *this; }
    virtual Page &endListItem() override { return *this; }
    virtual Page &beginKv(const SBuf &key) { atom(key) << ": "; return *this; }
    virtual Page &endKv() { *this << std::endl; return *this; }
    virtual Page &atom(const SBuf &text) { *this << text; return *this; }

private:
    int nesting; ///< list/hash nesting depth
};


Page::Page(std::ostream &os): std::ostream(os.rdbuf()) // XXX: this may be wrong
{
}

int
main(int, char*[]) {
    YamlPage page(std::cout);
    page.open();

    page.beginList();
    for (auto name: {"5min", "60min", "disk_io", "events", "shutdown"}) {
        page.beginListItem().
            beginHash().
                kv(name, "menu item description to be extracted from Action::desc").
                kv("protection", "public"). // ActionProtection(*a)
            endHash().
        endListItem();
    }
    page.endList();

    page.comment("This is not really related to the menu example above");
    page.beginKv("TestKey") << "several words " << "per " << 1 << " value " << "are OK";
    page.endKv();

    page.close();
    return 0;
}