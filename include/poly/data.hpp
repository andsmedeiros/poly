#ifndef POLY_DATA_HPP
#define POLY_DATA_HPP

#include <stdexcept>
#include "primitives.hpp"
#include "property.hpp"
#include "utils.hpp"

namespace poly {


struct data_conversion_error : public std::runtime_error {
    const data &source;

    data_conversion_error(const data &source, const std::string &from, const std::string &to) : 
        runtime_error { "Attempted to convert " + from + " data to " + to + "." },
        source { source }
        {  }
};

class data : public std::variant<null, string, integer, real, boolean, array, object> {
public:
    using variant::variant;
    using variant::operator=;

    data(integer value) noexcept;

    property operator[](const index_type &index);

    data &get(const index_type &index);
    data &set(const index_type &index, data value);
    bool has_property(const index_type &index) noexcept;

    template<class T>
    inline operator T&() {
        return as<T>();
    }

    template<class T>
    inline bool is_a() const noexcept { 
        static_assert(can_hold<T>(), "Value cannot ever be the requested type.");
        return std::holds_alternative<T>(*this); 
    }

    template<class T>
    inline T &as() {
        if(!is_a<T>()) {
            std::string from = std::visit([](auto &&value) { 
                return std::string { utils::type_name<decltype(value)>() }; 
            }, *this);
            std::string to { utils::type_name<T>() };
            throw data_conversion_error { *this, from, to };
        }
        return std::get<T>(*this);
    }

    template<class T>
    static inline constexpr bool can_hold() noexcept {
        return utils::variant_can_hold_v<T, variant>;
    }

private:
    void validate_index(const index_type &index) const;
    data &get_array_element(const array::index_type &index);
    data &get_object_property(const object::index_type &index);
};

inline data make_object(std::initializer_list<object::value_type> args = {}) {
    return object { std::begin(args), std::end(args) };
}

inline data make_array(std::initializer_list<array::value_type> args = {}) {
    return array { std::begin(args), std::end(args) };
}

} /* namespace poly */

#endif /* POLY_DATA_HPP */