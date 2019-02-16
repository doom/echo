/*
** Created by doom on 12/02/19.
*/

#ifndef ECHO_DETAILS_ARRAY_VIEW_HPP
#define ECHO_DETAILS_ARRAY_VIEW_HPP

#include <cstddef>

namespace echo::details
{
    template <typename T>
    class array_view
    {
    public:
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using pointer = T *;
        using const_pointer = const T *;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using size_type = std::size_t;

        array_view(T *data, size_type size) noexcept : data_(data), size_(size)
        {
        }

        array_view() noexcept : data_(nullptr), size_(0)
        {
        }

        array_view(const array_view &) = default;

        array_view(array_view &&) = default;

        array_view &operator=(const array_view &) = default;

        array_view &operator=(array_view &&) = default;

        iterator begin() noexcept
        {
            return data_;
        }

        iterator end() noexcept
        {
            return data_ + size_;
        }

        const_iterator begin() const noexcept
        {
            return data_;
        }

        const_iterator end() const noexcept
        {
            return data_ + size_;
        }

        size_type size() const noexcept
        {
            return size_;
        }

        reference operator[](size_type i)
        {
            return data_[i];
        }

        const_reference operator[](size_type i) const
        {
            return data_[i];
        }

    private:
        pointer data_;
        size_type size_;
    };
}

#endif /* !ECHO_DETAILS_ARRAY_VIEW_HPP */
