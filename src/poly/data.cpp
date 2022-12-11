#include "poly/data.hpp"

namespace poly {

data::data(integer value) noexcept:
    variant { value }
    {  }

property data::operator[](const index_type &index) {
    return { *this, index };
}

data &data::get(const index_type &index) {
    validate_index(index);

    if(is_a<array>()) {
        return get_array_element(std::get<array::index_type>(index));
    }

    if(is_a<object>()) {
        return get_object_property(std::get<object::index_type>(index));
    }

    throw property_access_error { *this, index, 
        "Data is in an insconsistent state."
    };
}

data &data::set(const index_type &index, data value) {
    validate_index(index);

    if(is_a<array>()) {
        return get_array_element(std::get<array::index_type>(index)) = value;
    }

    if(is_a<object>()) {
        return as<object>()[std::get<object::index_type>(index)] = value;
    }

    throw property_access_error { *this, index, 
        "Data is in an insconsistent state."
    };
}

bool data::has_property(const index_type &index) noexcept {
    if(is_a<array>() && std::holds_alternative<array::index_type>(index)) {
        return as<array>().size() > std::get<array::index_type>(index);
    }
    
    if(is_a<object>() && std::holds_alternative<object::index_type>(index)) {
        return as<object>().contains(std::get<object::index_type>(index));
    }

    return false;
}


void data::validate_index(const index_type &index) const {
    if(is_a<array>()) {
        if(!std::holds_alternative<array::index_type>(index)) {
            throw property_access_error { *this, index,
                "Attempted to access an array element with wrong index type."
            };
        }
    } else if(is_a<object>()) {
        if(!std::holds_alternative<object::index_type>(index)) {
            throw property_access_error { *this, index,
                "Attempted to access an object property with wrong index type."
            };
        }
    } else {
        throw property_access_error { *this, index,
            "Attempted to access property of scalar value."
        };
    }
}


data &data::get_array_element(const array::index_type &index) {
    auto &value = as<array>();
    if(index >= value.size()) {
        throw property_access_error { *this, index, 
            "Attempted to access an out-of-bounds array index."
        };
    }
    return value[index];
}

data &data::get_object_property(const object::index_type &index) {
    auto &value = as<object>();
    if(!value.contains(index)) {
        throw property_access_error { *this, index, 
            "Attempted to access an undefined object property."
        };
    }
    return value[index];

}


} /* namespace poly */