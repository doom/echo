/*
** Created by doom on 11/02/19.
*/

#ifndef ECHO_ECHO_HPP
#define ECHO_ECHO_HPP

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <string_view>
#include <echo/details/array_view.hpp>
#include <echo/details/string_view_hash.hpp>

#define reflectible     __attribute__((annotate("echo-class")))
#define reflect         __attribute__((annotate("echo-member")))

namespace echo
{
    enum type_tags
    {
        no_tag = 0x0,
        class_tag = 0x1,
        primitive_tag = 0x2,
    };

    struct type_descriptor
    {
        std::string_view name;
        std::size_t size;
        type_tags tags;
        const std::uint64_t hash;

        bool is_class() const noexcept
        {
            return (tags & class_tag) != no_tag;
        }

        bool is_primitive() const noexcept
        {
            return (tags & primitive_tag) != no_tag;
        }
    };

    template <typename T>
    const type_descriptor &get_type() noexcept;

    struct field_descriptor;

    namespace details
    {
        template <typename Field, typename Void>
        auto pointer_to_field(Void *obj, const field_descriptor &field_descriptor) noexcept;

        template <typename Field, typename Void>
        decltype(auto) reference_to_field(Void *obj, const field_descriptor &field_descriptor) noexcept;
    }

    template <typename T>
    struct typed_field_descriptor
    {
        using field_type = T;

        const field_descriptor &field_desc;
        const type_descriptor &type;
        std::string_view name;
        std::size_t offset;

        template <typename U>
        decltype(auto) get_from(U &t) const noexcept
        {
            return details::reference_to_field<T>(&t, field_desc);
        }
    };

    struct field_descriptor
    {
        const type_descriptor &type;
        std::string_view name;
        std::size_t offset;

        template <typename T>
        typed_field_descriptor<T> as() const
        {
            assert(type.name == get_type<T>().name);
            return {*this, type, name, offset};
        }
    };

    struct function_descriptor
    {
        type_descriptor return_type;
        std::string_view name;
        details::array_view<const field_descriptor> parameters;

        const field_descriptor &get_parameter(std::size_t i) const
        {
            assert(i < parameters.size());
            return parameters[i];
        }

        auto find_parameter_by_name(const std::string_view name) const noexcept
        {
            for (auto field_it = parameters.begin(); field_it != parameters.end(); ++field_it) {
                if (field_it->name == name)
                    return field_it;
            }
            return parameters.end();
        }

        const field_descriptor &get_parameter_by_name(const std::string_view name) const
        {
            auto field_it = find_parameter_by_name(name);

            assert(field_it != parameters.end());
            return *field_it;
        }
    };

    struct class_descriptor : type_descriptor
    {
        details::array_view<const field_descriptor> fields;
        details::array_view<const function_descriptor> functions;

        auto find_field_by_name(const std::string_view name) const noexcept
        {
            for (auto field_it = fields.begin(); field_it != fields.end(); ++field_it) {
                if (field_it->name == name)
                    return field_it;
            }
            return fields.end();
        }

        const field_descriptor &get_field_by_name(const std::string_view name) const
        {
            auto field_it = find_field_by_name(name);

            assert(field_it != fields.end());
            return *field_it;
        }

        auto find_function_by_name(const std::string_view name) const noexcept
        {
            for (auto function_it = functions.begin(); function_it != functions.end(); ++function_it) {
                if (function_it->name == name)
                    return function_it;
            }
            return functions.end();
        }

        const function_descriptor &get_function_by_name(const std::string_view name) const
        {
            auto function_it = find_function_by_name(name);

            assert(function_it != functions.end());
            return *function_it;
        }
    };

#define FOR_EACH_PRIMITIVE_TYPE(F)                                      \
    F(char);                                                            \
    F(unsigned char);                                                   \
    F(short);                                                           \
    F(unsigned short);                                                  \
    F(int);                                                             \
    F(unsigned int);                                                    \
    F(long);                                                            \
    F(unsigned long);                                                   \
    F(long long);                                                       \
    F(unsigned long long);                                              \
    F(float);                                                           \
    F(double);                                                          \
    F(long double);                                                     \
    F(bool);

    template <typename T>
    const type_descriptor &get_type() noexcept;

#define GENERATE_ECHO_TYPE(T)                                           \
    template <>                                                         \
    inline const type_descriptor &get_type<T>() noexcept                \
    {                                                                   \
        static const type_descriptor desc{                              \
            #T,                                                         \
            sizeof(T),                                                  \
            type_tags::primitive_tag,                                   \
            details::hash(#T),                                          \
        };                                                              \
                                                                        \
        return desc;                                                    \
    }

    FOR_EACH_PRIMITIVE_TYPE(GENERATE_ECHO_TYPE);

#undef GENERATE_ECHO_TYPE

    template <typename T>
    const type_descriptor &get_type() noexcept
    {
        static const type_descriptor desc{
            "void",
            0,
            type_tags::primitive_tag,
            details::hash("void"),
        };

        return desc;
    }

    template <typename T>
    const class_descriptor &get_class() noexcept;

    namespace details
    {
        template <typename Field, typename Void>
        auto pointer_to_field(Void *obj, const field_descriptor &field_descriptor) noexcept
        {
            constexpr bool is_const = std::is_const_v<std::remove_reference_t<Void>>;
            using target_ptr = std::add_pointer_t<std::conditional_t<is_const, std::add_const_t<Field>, Field>>;
            using char_ptr = std::conditional_t<is_const, const char *, char *>;

            return reinterpret_cast<target_ptr>(reinterpret_cast<char_ptr>(obj) + field_descriptor.offset);
        }

        template <typename Field, typename Void>
        decltype(auto) reference_to_field(Void *obj, const field_descriptor &field_descriptor) noexcept
        {
            return *pointer_to_field<Field>(obj, field_descriptor);
        }
    }
}

#endif /* !ECHO_ECHO_HPP */
