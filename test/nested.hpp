/*
** Created by doom on 15/02/19.
*/

#ifndef TEST_NESTED_HPP
#define TEST_NESTED_HPP

#include <echo/echo.hpp>

struct reflectible contained
{
    reflect unsigned int i;
};

struct reflectible container
{
    reflect contained a;
    reflect contained b;
};

#ifndef ECHOGEN_GENERATION_PASS
#include "nested.echo.hpp"
#endif /* !ECHOGEN_GENERATION_PASS */

#endif /* !TEST_NESTED_HPP */
