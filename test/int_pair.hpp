/*
** Created by doom on 12/02/19.
*/

#ifndef TEST_INT_PAIR_HPP
#define TEST_INT_PAIR_HPP

#include <echo/echo.hpp>

struct reflectible int_pair
{
public:
    reflect int first;
    reflect int second;
};

#ifndef ECHOGEN_GENERATION_PASS
#include "int_pair.echo.hpp"
#endif /* !ECHOGEN_GENERATION_PASS */

#endif /* !TEST_INT_PAIR_HPP */
