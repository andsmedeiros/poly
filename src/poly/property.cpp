#include "poly/property.hpp"
#include "poly/data.hpp"

namespace poly {

property::property(data &source, const index_type &index) :
    source { source },
    index { index }
    {  }

data &property::get() const { 
    return source.get(index);
}

data &property::set(data value) const {
    return source.set(index, std::forward<decltype(value)>(value));
}

bool property::is_defined() const noexcept {
    return source.has_property(index);
}

property property::operator[](const index_type &index) const {
    return get()[index];
}

} /* namespace poly */