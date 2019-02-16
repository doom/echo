/*
** Created by doom on 14/02/19.
*/

#ifndef ECHO_DEBUG_HPP
#define ECHO_DEBUG_HPP

#include <ostream>
#include "echo.hpp"

namespace echo::debug
{
    template <typename T>
    struct debug
    {
        const T &ref;
    };

    template <typename T>
    debug(T &) -> debug<T>;

    namespace details
    {
        inline void indent_for(std::ostream &os, std::size_t depth) noexcept
        {
            while (depth--) {
                os << "    ";
            }
        }

#define GENERATE_CASE(T)                                    \
    case hash(#T):                                          \
        os << reference_to_field<const T>(raw_data, f);     \
        break;

        inline void dump_primitive_field(std::ostream &os, const field_descriptor &f, const void *raw_data)
        {
            using echo::details::hash;
            using echo::details::reference_to_field;

            os << f.name << ": ";
            switch (f.type.hash) {
                FOR_EACH_PRIMITIVE_TYPE(GENERATE_CASE);
                default:
                    break;
            }
        }

#undef GENERATE_CASE

        inline void dump_class(std::ostream &os, const void *raw_data,
                               const class_descriptor &class_desc, std::size_t depth = 0);

        inline void dump_field(std::ostream &os, const field_descriptor &f, const void *raw_data, std::size_t depth)
        {
            using echo::details::pointer_to_field;

            indent_for(os, depth);
            if (f.type.is_primitive()) {
                dump_primitive_field(os, f, raw_data);
            } else {
                os << f.name << ": ";
                dump_class(os, pointer_to_field<const void>(raw_data, f), static_cast<const class_descriptor &>(f.type), depth);
            }
        }

        inline void dump_class(std::ostream &os, const void *raw_data,
                               const class_descriptor &class_desc, std::size_t depth)
        {
            os << class_desc.name << " {\n";
            for (const auto &field : class_desc.fields) {
                details::dump_field(os, field, raw_data, depth + 1);
                os << ",\n";
            }
            indent_for(os, depth);
            os << "}";
        }
    }

    template <typename T>
    inline std::ostream &operator<<(std::ostream &os, const debug<T> &d)
    {
        details::dump_class(os, reinterpret_cast<const void *>(&d.ref), get_class<T>());
        return os;
    }
}

#endif /* !ECHO_DEBUG_HPP */
