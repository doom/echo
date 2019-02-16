/*
** Created by doom on 16/02/19.
*/

#ifndef ECHO_DETAILS_STRING_VIEW_HASH_HPP
#define ECHO_DETAILS_STRING_VIEW_HASH_HPP

#include <string_view>

namespace echo::details
{
    constexpr uint64_t hash(const std::string_view sv) noexcept
    {
        uint64_t h = 0;

        for (const auto &c : sv) {
            h = (h << 5) + h + c;
        }
        return h;
    }
}

#endif /* !ECHO_DETAILS_STRING_VIEW_HASH_HPP */
