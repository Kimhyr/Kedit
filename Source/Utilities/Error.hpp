#pragma once
#ifndef ERROR_HPP
#define ERROR_HPP

#include <Core.hpp>

trait Error {
    virtual function description() const -> Byte const* = 0;
};

class Out_Of_Range
    : public Error {
public:
    constexpr Out_Of_Range(Byte const* description)
        : m_description(description) {}

    ~Out_Of_Range() = default;

public:
    function description() const -> Byte const* override { return m_description; }

private:
    Byte const* m_description;
};

#endif

