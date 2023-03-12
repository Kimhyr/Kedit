#pragma once
#ifndef KEDIT_BUCKET_HPP
#define KEDIT_BUCKET_HPP

#include <Core.hpp>
#include <Utilities/View.hpp>

template<typename Item_T, Nat Capacity_T>
class Fixed_Buffer {
public:
    using This = Fixed_Buffer;
    using Item = Item_T;

    static constexpr Nat const CAPACITY = Capacity_T;

public:
    Fixed_Buffer() = default;
    Fixed_Buffer(This&&) = delete;
    Fixed_Buffer(This const& other);

    function operator=(This&&) -> This& = delete;
    function operator=(This const&) -> This&;

    ~Fixed_Buffer() = default;

public:
    property begin() const -> Item const* { return m_data; }
    property end() const -> Item const* { return &m_data[CAPACITY]; }
    property capacity() const -> Nat { return CAPACITY; }
    property ptr() const -> Item const* { return m_ptr; }
    property ptr_position() const -> Nat { return m_ptr - m_data; }

public:
    /// Puts a string of `Item`s into `m_data` from `m_ptr`.
    /// \return The amount of items putten.
    function put(View<Item> const& items) -> Nat;

    /// Takes `difference` amount of items.
    /// \return The amount of items taken.
    function take(Nat difference) -> Nat;

private:
    Item m_data[CAPACITY];
    Item* m_ptr = m_data;
};

#endif
