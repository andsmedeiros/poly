#include <poly.hpp>
#include <poly/utils.hpp>
#include <iostream>
#include <cassert>
#include <memory>
#include <utility>

int main(int argc, char const *argv[]) {
    poly::data d1 = 1, d2 = false, d3 = nullptr;
    poly::data a1 = poly::make_array({ d1, d2, d3, 27, false, nullptr, "abc" });
    poly::data o1 = poly::make_object({
        { "true", true },
        { "array", a1 },
        { "subobject", poly::make_object({
            { "a", 1 },
            { "b", 3.1415 },
            { "inline-array", poly::make_array({ 1024, nullptr, poly::make_array(), poly::make_object() }) }
        }) }
    });

    o1["null"] = poly::data {  };
    bool is_null = o1["null"].as<poly::null>() == nullptr;
    assert(("o1.null must be null", is_null));
    bool is_one_deep = o1["subobject"]["a"].as<poly::integer>() == 1;
    poly::integer num = o1["subobject"]["inline-array"][0];

    assert(("o1 'false' property must be undefined", !o1.has_property("false")));
    assert(("o1 'false' property must be undefined", !o1["false"].is_defined()));
    poly::data empty;
    
    volatile bool b = empty.is_a<poly::null>();

    []<class... T_args>(std::variant<T_args ...> &) {
        ((std::cout << poly::utils::type_name<T_args>() << ": " << sizeof(T_args) << std::endl),...);
    }(d1);
    std::cout << poly::utils::type_name<std::shared_ptr<poly::object>>() << ": " << sizeof(std::shared_ptr<poly::object>) << std::endl;
    std::cout << poly::utils::type_name<std::shared_ptr<poly::data>>() << ": " << sizeof(std::shared_ptr<poly::data>) << std::endl;
    std::cout << poly::utils::type_name<poly::data>() << ": " << sizeof(poly::data) << std::endl;
    std::cout << poly::utils::type_name<poly::object::value_type>() << ": " << sizeof(poly::object::value_type) << std::endl;

    poly::object obj { { "a", 1 }, { "b", 2 }, { "c", false }, { "d" , nullptr } };

    return 0;
}
