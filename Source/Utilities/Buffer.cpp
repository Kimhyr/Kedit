#include <Utilities/Buffer.hpp>

template<typename A_T, Nat B_T>
Fixed_Buffer<A_T, B_T>::Fixed_Buffer(This const& other) {
    for (Item const& item : other)
        *m_ptr++ = item;
}

template<typename A_T, Nat B_T>
function Fixed_Buffer<A_T, B_T>::operator=(This const& other) -> This& {
    for (Item const& item : other)
        *m_ptr++ = item;
}
