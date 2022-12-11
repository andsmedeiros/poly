#ifndef POLY_PROPERTY_HPP
#define POLY_PROPERTY_HPP

#include <stdexcept>
#include <string>
#include "primitives.hpp"

namespace poly {

struct property_access_error : public std::runtime_error {
    const data &source;
    const index_type index;

    property_access_error(const data &source, index_type index, std::string cause) : 
        runtime_error { std::move(cause) },
        source { source },
        index { index }
        {  }
};

class property {
    data &source;
    const index_type index;

public:
    property(data &source, const index_type &index);

    data &get() const;
    data &set(data value) const;
    bool is_defined() const noexcept;

    property operator[](const index_type &index) const;

    template<class T>
    inline data &operator=(T&& value) {
        return set(std::forward<T>(value));
    }

    template<class T>
    inline operator T&() {
        return static_cast<T&>(get());
    }

    template<class T>
    inline T &as() {
        return static_cast<T &>(get());
    }
};


} /* namespace poly */

#endif /* POLY_PROPERTY_HPP */