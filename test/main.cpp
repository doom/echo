/*
** Created by doom on 11/02/19.
*/

#include <cassert>
#include <iostream>
#include <echo/debug.hpp>
#include "int_pair.hpp"
#include "vec_2d.hpp"
#include "nested.hpp"

int main()
{
    // Get basic type information, available for both primitive and echo'ed types
    auto &int_pair_type = echo::get_type<int_pair>();
    assert(int_pair_type.name == "int_pair");
    assert(int_pair_type.size == sizeof(int_pair));

    // Get extended type information, available only for echo'ed class types
    auto &int_pair_class = echo::get_class<int_pair>();
    assert(int_pair_class.name == "int_pair");
    assert(int_pair_class.size == sizeof(int_pair));

    for (const auto &f : int_pair_class.fields) {
        std::cout << f.name << std::endl;
    }

    // Get field descriptor by name, and add type information to it
    auto first_field = int_pair_class.get_field_by_name("first").as<int>();
    int_pair pair{1, 2};

    // Get a reference to the field of an instance
    auto &first_ref = first_field.get_from(pair);
    assert(&first_ref == &pair.first);
    first_ref = 421;
    assert(pair.first == 421);

    const int_pair pair2{1, 2};
    const auto &first_ref2 = first_field.get_from(pair2);
    assert(&first_ref2 == &pair2.first);

    vec_2d v1{2, 2};
    auto &vec_2d_class = echo::get_class<vec_2d>();

    for (const auto &f : vec_2d_class.fields) {
        std::cout << f.name << ", of type " << f.type.name << ", at offset " << f.offset << std::endl;
    }

    for (const auto &f : vec_2d_class.functions) {
        std::cout << f.name << std::endl;
    }

    using namespace echo::debug;

    std::cout << debug{v1} << std::endl;
    container c1{{12}, {34}};
    std::cout << debug{c1} << std::endl;
}
