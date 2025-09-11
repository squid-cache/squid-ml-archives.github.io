#ifndef SQUID_BASE_ENUMITERATOR_H
#define SQUID_BASE_ENUMITERATOR_H
/*
 * Copyright (C) 1996-2015 The Squid Software Foundation and contributors
 *
 * Squid software is distributed under GPLv2+ license and includes
 * contributions from numerous individuals and organizations.
 * Please see the COPYING and CONTRIBUTORS files for details.
 */

#include <iterator>
#include <type_traits>

/* code shared between forward and reverse iterators */
template <typename Enum>
class EnumIteratorBase : public std::iterator<std::bidirectional_iterator_tag, Enum>
{
public:
    explicit EnumIteratorBase(Enum e) : current(static_cast<iterator_type>(e)) {}

    bool operator==(const EnumIteratorBase &i) const {
        return current == i.current;
    }

    bool operator!=(const EnumIteratorBase &i) const {
        return current != i.current;
    }

    Enum operator*() const {
        return static_cast<Enum>(current);
    }

protected:
    typedef typename std::underlying_type<Enum>::type iterator_type;
    iterator_type current;
};

/** iterator over an enum type
 *
 */
template <typename Enum>
class EnumIterator : public EnumIteratorBase<Enum>
{
public:
    explicit EnumIterator(Enum e) : EnumIteratorBase<Enum>(e) {}

    // prefix increment
    EnumIterator& operator++() {
        ++ EnumIteratorBase<Enum>::current;
        return *this;
    }

    // postfix increment
    EnumIterator& operator++(int) {
        EnumIterator rv(*this);
        ++ EnumIteratorBase<Enum>::current;
        return rv;
    }

    // prefix decrement
    EnumIterator& operator--() {
        -- EnumIteratorBase<Enum>::current;
        return *this;
    }

    // postfix decrement
    EnumIterator& operator--(int) {
        EnumIterator rv(*this);
        -- EnumIteratorBase<Enum>::current;
        return rv;
    }
};

template <typename Enum>
class ReverseEnumIterator : public EnumIteratorBase<Enum>
{
public:
    explicit ReverseEnumIterator(Enum e) : EnumIteratorBase<Enum>(e) {}

    // prefix increment
    ReverseEnumIterator& operator++() {
        -- EnumIteratorBase<Enum>::current;
        return *this;
    }

    // postfix increment
    ReverseEnumIterator& operator++(int) {
        ReverseEnumIterator rv(*this);
        -- EnumIteratorBase<Enum>::current;
        return rv;
    }

    // prefix decrement
    ReverseEnumIterator& operator--() {
        ++ EnumIteratorBase<Enum>::current;
        return *this;
    }

    // postfix decrement
    ReverseEnumIterator& operator--(int) {
        ReverseEnumIterator rv(*this);
        ++ EnumIteratorBase<Enum>::current;
        return rv;
    }
};

/** Class expressing a range of an enum
 *
 * This class is suited to use range-for over a continuous portion of an enum.
 * This class requires that the underlying enum values be represented by
 * continuous values of an integral type.
 * Typical use:
 * for ( auto enumvalue : EnumRangeT<EnumType>(EnumType::somevalue,
 *                        EnumType::someOtherValue) )
 * { do_stuff(); }
 * Note that EnumIterator<enum>(EnumType::firstmember,EnumType::lastmember)
 * will miss EnumType::lastmember while iterating. If you need to iterate
 * over all of EnumType, use class WholeEnum. Otherwise you'll have to
 * explicitly address lastmember outside the iteration loop.
 */
template <typename Enum>
class EnumRangeT
{
public:
    typedef EnumIterator<Enum> iterator;
    typedef ReverseEnumIterator<Enum> reverse_iterator;
    EnumRangeT(Enum first, Enum one_past_last) : begin_(first), end_(one_past_last) { }
    iterator begin() const { return iterator(begin_);}
    iterator end() const { return iterator(end_);}
    reverse_iterator rbegin() const { return ++reverse_iterator(end_); }
    reverse_iterator rend() const { return ++reverse_iterator(begin_); }
private:
    Enum begin_;
    Enum end_;
};

/// convenience function to deduce the right type for instantiating EnumRangeT
template <typename Enum>
EnumRangeT<Enum> EnumRange(Enum begin, Enum one_past_end)
{
    return EnumRangeT<Enum>(begin,one_past_end);
}

/** Class for iterating all enum values, from Enum::enumBegin_ up to, but
 *  not including, Enum::enumEnd_. Both markers must be present as enum values.
 *
 * This class requires that the underlying enum values be represented by
 * continuous values of an integral type.
 * Typical use: for( auto enumvalue : WholeEnum<EnumType>() ) { do_stuff(); }
 */
template <typename Enum>
class WholeEnum : public EnumRangeT<Enum>
{
public:
    WholeEnum() : EnumRangeT<Enum>(Enum::enumBegin_, Enum::enumEnd_) {}
};

#endif /* SQUID_BASE_ENUMITERATOR_H */
