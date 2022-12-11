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

class data : 
    public std::variant<null, integer, real, boolean, string_ptr, array_ptr, object_ptr> 
{

public:
    using variant::variant;
    using variant::operator=;

    data(null) noexcept;
    data(integer value) noexcept;
    data(const string &value);
    
    template<std::size_t Size> 
    data(const char (& value)[Size]) : 
        variant { std::make_shared<string>(value, Size) }
        {  }

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
        if constexpr(is_scalar<T>()) {
            return std::holds_alternative<T>(*this);
        } else {
            return std::holds_alternative<std::shared_ptr<T>>(*this); 
        }
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

        if constexpr(is_scalar<T>()) {
            return std::get<T>(*this);
        } else {
            return *std::get<std::shared_ptr<T>>(*this);
        }
    }

    template<class T>
    static inline constexpr bool can_hold() noexcept {
        return utils::variant_can_hold_v<T, variant> ||
            utils::variant_can_hold_v<std::shared_ptr<T>, variant>;
    }

private:
    void validate_index(const index_type &index) const;
    data &get_array_element(const array::index_type &index);
    data &get_object_property(const object::index_type &index);
};

inline data make_object(std::initializer_list<object::value_type> args = {}) {
    return std::make_shared<object>(std::begin(args), std::end(args));
}

inline data make_array(std::initializer_list<array::value_type> args = {}) {
    return std::make_shared<array>(std::begin(args), std::end(args));
}

} /* namespace poly */

#endif /* POLY_DATA_HPP */