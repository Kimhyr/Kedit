#pragma once
#ifndef VIEW_HPP
#define VIEW_HPP

#include <Core.hpp>
#include <Utilities/Error.hpp>

template<typename Item_T>
class View {
public:
    using This = View;
    using Item = Item_T;

public:
    constexpr View() = default;
    constexpr View(This&& other) : m_begin(other.begin()), m_end(other.end()) { other.m_end = other.m_begin = nil; }
    constexpr View(This const& other) = default;
    constexpr View(Item const* begin, Item const* end) : m_begin(begin), m_end(end) {}

    View(decltype(nil)) = delete;

    constexpr This& operator=(This&& other) {
        m_begin = other.begin();
        m_end = other.end();
        other.m_end = other.m_begin = nil;
    }

    constexpr This& operator=(This const& other) = default;

    ~View() = default;

public:
    property begin() const -> Item const* { return m_begin; }
    property rbegin() const -> Item const* { return m_end - 1; }
    property end() const -> Item const* { return m_end; }
    property rend() const -> Item const* { return m_begin - 1; }

    property mass() const -> Nat { return m_begin - m_end; }
    property is_empty() const -> Bit { return begin() == end(); }

    property data() const -> Item const* { return begin();}
    property front() const -> Item const& { return *begin(); }
    property back() const -> Item const& { return end()[-1]; }
    property operator[](Int index) -> Item const& { return data()[index]; }
    property at(Nat index) -> Item const& {
        if (index >= length())
            throw Out_Of_Range("");
        return data()[index];
    }

public:
    function swap_contents(View& other) {
        if (other.begin() == begin())
            return;
        // TODO: this.
        m_begin ^= other.begin();
        other.m_begin ^= m_begin;
        m_begin ^= other.begin();
     }

    inline function shift_prefix(Nat difference) { m_begin += difference; }
    inline function shift_suffix(Nat difference) { m_end -= difference; }

private:
    Item const* m_begin = nil;
    Item const* m_end = nil;
};

#endif
