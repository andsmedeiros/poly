#ifndef POLY_PRIMITIVES_HPP
#define POLY_PRIMITIVES_HPP

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <type_traits>

namespace poly {

class data;

using null = std::nullptr_t;
using integer = std::int64_t;
using real = double;
using boolean = bool;

using string = std::string;
using string_ptr = std::shared_ptr<string>;

class object : public std::map<std::string, data> {
    friend inline poly::data make_object(auto &&...args);
    using map::map;

public:
    using index_type = map::key_type;
};
using object_ptr = std::shared_ptr<object>;

class array : public std::vector<data> {
    friend inline poly::data make_array(auto &&...args);
    using vector::vector;

public:
    using index_type = vector::size_type;
};
using array_ptr = std::shared_ptr<array>;

struct index_type : public std::variant<object::index_type, array::index_type> {
    using variant::variant;

    index_type(int index) : 
        variant { static_cast<array::index_type>(index) } {  }
};

template<class T>
inline constexpr bool is_aggregate() noexcept {
    return
        std::is_same_v<T, string> ||
        std::is_same_v<T, object> ||
        std::is_same_v<T, array>;
}

template<class T>
inline constexpr bool is_scalar() noexcept {
    return 
        std::is_same_v<T, null>     ||
        std::is_same_v<T, integer>  ||
        std::is_same_v<T, real>     ||
        std::is_same_v<T, boolean>;
}

} /* namespace poly */

#endif /* POLY_PRIMITIVES_HPP */