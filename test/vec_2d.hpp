/*
** Created by doom on 13/02/19.
*/

#ifndef ECHO_VEC_2D_HPP
#define ECHO_VEC_2D_HPP

#include <echo/echo.hpp>
#include <cmath>

struct reflectible vec_2d
{
    reflect double x;
    reflect double y;

    reflect double norm() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    reflect void scale(double f) noexcept
    {
        x *= f;
        y *= f;
    }
};

#ifndef ECHOGEN_GENERATION_PASS
#include "vec_2d.echo.hpp"
#endif /* !ECHOGEN_GENERATION_PASS */

#endif /* !ECHO_VEC_2D_HPP */
