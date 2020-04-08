#pragma once

#include <algorithm>
#include <memory>

namespace low
{
    template <typename It, typename = void>
    struct IsIterable : std::false_type{};

    template <typename It>
    struct IsIterable<It, std::void_t<typename std::iterator_traits<It>::value_type>> : std::true_type{};

    template <typename T, typename Alloc = std::allocator<T>>
    class vector
    {
        // meet AllocatorAwareContainer requirements;
        // see https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer#Requirements
        static_assert(std::is_same_v<T, typename Alloc::value_type>, "vector element's type should be same as for allocator");

    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = pointer;
        using const_iterator = const_pointer;
        // employ user-provided allocator, since we ensured that it value_type is same as for vector,
        // thus no need to rebound allocator
        using allocator_type = Alloc;
        using allocator_traits = std::allocator_traits<Alloc>;

        vector() = default;

        explicit vector(const allocator_type &alloc) noexcept : alloc_(alloc) {}

        vector(vector &&r) noexcept :
            alloc_(std::move(r.alloc_))
        {
            steal_pointers(std::move(r));
        }

        vector(vector &&r, const allocator_type &alloc)
            noexcept(allocator_traits::is_always_equal::value) : alloc_(alloc)
        {
            // compile time knowledge, that allocators always equal
            if constexpr (allocator_traits::is_always_equal::value)
            {
                // if so, just steal a pointers
                steal_pointers(std::move(r));
                return;
            }
            // otherwise perform runtime dispatch
            if (r.get_allocator() == alloc)
            {
                // just steal a pointers
                steal_pointers(std::move(r));
            }
            else
            {
                // perform element-wise move of stored objects
                reserve(r.size());
                end_ = std::uninitialized_move(r.begin_, r.end_, begin_);
            }
        }

        vector(const vector &r) :
            vector(r, allocator_traits::select_on_container_copy_construction(r.get_allocator()))
        {}

        vector(const vector &r, const allocator_type &alloc) :
            alloc_(alloc)
        {
            reserve(r.size());
            end_ = std::uninitialized_copy(r.begin_, r.end_, begin_);
        }

        vector(const std::initializer_list<value_type> &list)
        {
            reserve(list.size());
            end_ = std::uninitialized_copy(list.begin(), list.end(), begin_);
        }

        vector &operator=(vector &&r) noexcept(is_equal_or_pocma_v)
        {
            if (this == &r) // be on a safe side
            {
                return *this;
            }
            // compile time knowledge, that we need to steal resources from other
            if constexpr (is_equal_or_pocma_v)
            {
                move_assign_impl(std::move(r));
                return *this;
            }
            // if there is no compile time properties, perform runtime dispatch:
            if (get_allocator() == r.get_allocator())
            {
                move_assign_impl(std::move(r));
            }
            else
            {
                // otherwise, perform element-wise move
                assign(move_iterator_if_noexcept_t{r.begin()}, move_iterator_if_noexcept_t{r.end()});
            }
            return *this;
        }

        vector &operator=(const vector &r)
        {
            if (this == &r) // be on a safe side
            {
                return *this;
            }
            // in this case other alloc unable to deal with elements from *this, so deallocate it
            if (allocator_traits::propagate_on_container_copy_assignment::value && get_allocator() != r.get_allocator())
            {
                release_storage();
            }
            // compile time knowledge - propagate other allocator or not
            if constexpr (allocator_traits::propagate_on_container_copy_assignment::value)
            {
                alloc_ = r.alloc_;
            }
            // and assign other elements using old or new allocator, depending on pocca
            assign(r.begin(), r.end());
            return *this;
        }

        template <typename It>
        void assign(It first, It last)
        {
            const auto distance = static_cast<std::size_t>(std::distance(first, last));
            if (capacity() < distance)
            {
                // if capacity is not enough - allocate it
                release_storage();
                reserve(distance);
            }

            // copy-assign elements until either:
            // 1) all elements from [first, last) copied or
            // 2) elements in *this are over
            const It pivot = first + static_cast<std::ptrdiff_t>(std::min(size(), distance));
            const iterator end = std::copy(first, pivot, begin_);

            // destruct old remaining elements if there are any
            destroy(end, end_);

            // copy-construct the rest elements from initial [first, last)
            // range if there are any
            end_ = std::uninitialized_copy(pivot, last, end);
        }

        void insert(const_iterator pos, std::size_t count, const value_type &value)
        {
            return insert_impl(pos, count, value);
        }

        void insert(const_iterator pos, const value_type &value)
        {
            return insert_impl(pos, 1, value);
        }

        void insert(const_iterator pos, value_type &&value)
        {
            return insert_impl(pos, 1, std::move(value));
        }

        template <typename It, typename = std::enable_if_t<IsIterable<It>::value>>
        void insert(const_iterator const_pos, It first, It last)
        {
            iterator pos = begin_ + std::distance(const_iterator{begin_}, const_pos);
            auto distance = static_cast<std::size_t>(std::distance(first, last));
            if (capacity() - size() < distance)
            {
                // capacity is not enough, so allocate new memory and insert all there
                std::size_t new_capacity{size() + distance};
                auto new_begin = allocator_traits::allocate(alloc_, new_capacity);

                auto new_end = uninitialized_move_if_noexcept(begin_, pos, new_begin);
                new_end = std::uninitialized_copy(first, last, new_end);
                new_end = uninitialized_move_if_noexcept(pos, end_, new_end);

                release_storage();

                begin_ = new_begin;
                end_ = new_end;
                capacity_ = new_begin + new_capacity;
            }
            else if (distance > 0)
            {
                // capacity is enough, insert data in place
                auto elements_in_self_to_move_count = static_cast<std::size_t>(end_ - pos);
                auto to_uninit_move_count = std::min(elements_in_self_to_move_count, distance);

                const auto pivot1 = end_ - to_uninit_move_count;
                uninitialized_move_backward(pivot1, end_, end_ + distance);
                std::move_backward(pos, pivot1, end_);

                const auto pivot2 = first + static_cast<std::ptrdiff_t>(to_uninit_move_count);
                std::copy(first, pivot2, pos);
                std::uninitialized_copy(pivot2, last, pos + to_uninit_move_count);

                end_ += distance;
            }
        }

        template <typename ...Args>
        reference emplace_back(Args&& ...args)
        {
            if (auto current_size = size(); current_size == capacity())
            {
                std::size_t new_capacity = get_next_capacity_or(current_size);
                reallocate_storage(new_capacity, current_size);
            }
            allocator_traits::construct(alloc_, end_, std::forward<Args>(args)...);
            return *end_++;
        }

        void pop_back()
        {
            allocator_traits::destroy(alloc_, --end_);
        }

        void reserve(std::size_t new_capacity)
        {
            if (new_capacity > capacity())
            {
                reallocate_storage(new_capacity, size());
            }
        }

        void resize(std::size_t new_size) { resize_impl(new_size); }
        void resize(std::size_t new_size, const_reference value) { resize_impl(new_size, value); }

        reference operator[](std::size_t idx)
        {
            return *(begin_ + idx);
        }

        const_reference operator[](std::size_t idx) const
        {
            return *(begin_ + idx);
        }

        std::size_t size() const noexcept { return static_cast<std::size_t>(end_ - begin_); }
        std::size_t capacity() const noexcept { return static_cast<std::size_t>(capacity_ - begin_); }
        bool empty() const noexcept { return size() == 0u; }

        iterator begin() noexcept { return begin_; }
        iterator end() noexcept { return end_; }

        const_iterator begin() const noexcept { return begin_; }
        const_iterator end() const noexcept { return end_; }

        const_iterator cbegin() const noexcept { return begin_; }
        const_iterator cend() const noexcept { return end_; }

        const allocator_type &get_allocator() const noexcept { return alloc_; }

        iterator erase(const_iterator first, const_iterator last)
        {
            if (first != last)
            {
                auto first_mutable = const_cast<iterator>(first);
                auto last_mutable = const_cast<iterator>(last);
                end_ = destroy(std::move(last_mutable, end_, first_mutable), end_);
            }
            return end_;
        }

        iterator erase(const_iterator pos)
        {
            // pos is forbidden to be end()
            return erase(pos, std::next(pos));
        }

        void clear() noexcept
        {
            end_ = destroy(begin_, end_);
        }

        void shrink_to_fit() noexcept
        {
            if (std::size_t current_size = size(); current_size == 0u)
            {
                release_storage();
            }
            else if (current_size != capacity())
            {
                reallocate_storage(current_size, current_size);
            }
        }

        ~vector() noexcept { release_storage(); }

    private:
        // weak mean if move ctor noexcept - use it; but if it is not, and there is no copy ctor, then
        // use throwing move ctor anyway. In last case strong exception guarantee may be violated.
        // See std::move_if_noexcept
        constexpr static bool is_nothrow_move_constructible_weak_v =
            std::is_nothrow_move_constructible_v<value_type> || !std::is_copy_constructible_v<value_type>;
        using move_iterator_if_noexcept_t = std::conditional_t<
            is_nothrow_move_constructible_weak_v, std::move_iterator<iterator>, iterator>;

        constexpr static bool is_equal_or_pocma_v = allocator_traits::is_always_equal::value ||
                                                  allocator_traits::propagate_on_container_move_assignment::value;

        static iterator uninitialized_move_if_noexcept(iterator first, iterator last, iterator result)
        {
            if constexpr (is_nothrow_move_constructible_weak_v)
                return std::uninitialized_move(first, last, result);
            else
                return std::uninitialized_copy(first, last, result);
        }

        template <typename InputIt, typename OutputIt>
        static OutputIt uninitialized_move_backward(InputIt first, InputIt last, OutputIt result)
        {
            while (first < last)
            {
                new(--result) value_type{std::move(*--last)};
            }
            return result;
        }

        template<typename U>
        void insert_impl(const_iterator const_pos, std::size_t count, U &&value)
        {
            static_assert(std::is_same_v<std::decay_t<U>, value_type>, "'value' is just universal reference");

            iterator pos = begin_ + std::distance(const_iterator{begin_}, const_pos);
            if (capacity() - size() < count)
            {
                // capacity is not enough, so allocate new memory and insert all there
                std::size_t new_capacity{size() + count};
                auto new_begin = allocator_traits::allocate(alloc_, new_capacity);

                auto new_end = uninitialized_move_if_noexcept(begin_, pos, new_begin);
                for (; count > 0; --count)
                {
                    allocator_traits::construct(alloc_, new_end++, std::forward<U>(value));
                }
                new_end = uninitialized_move_if_noexcept(pos, end_, new_end);

                release_storage();

                begin_ = new_begin;
                end_ = new_end;
                capacity_ = new_begin + new_capacity;
            }
            else
            {
                // capacity is enough, insert data in place
                auto elements_in_self_to_move_count = static_cast<std::size_t>(end_ - pos);
                auto to_uninit_move_count = std::min(elements_in_self_to_move_count, count);

                const auto pivot1 = end_ - to_uninit_move_count;
                uninitialized_move_backward(pivot1, end_, end_ + count);
                std::move_backward(pos, pivot1, end_);

                if (count == 1)
                {
                    allocator_traits::construct(alloc_, pos, std::forward<U>(value));
                }
                else
                {
                    std::fill_n(pos, to_uninit_move_count, value);
                    std::uninitialized_fill_n(pos + to_uninit_move_count, count - to_uninit_move_count, value);
                }
                end_ += count;
            }
        }

        template <typename ...U>
        void resize_impl(std::size_t new_size, const U& ...value)
        {
            static_assert(sizeof...(U) <= 1);
            static_assert((std::is_same_v<U, value_type> && ...),
                    "if 'value' present, then it should be of 'value_type' type");

            if (iterator it{begin_ + new_size}; it < end_)
            {
                // user provide size smaller than current size,
                // so we have to truncate vector and return
                end_ = destroy(it, end_);
                return;
            }
            // allocate new capacity if needed
            if (capacity() < new_size)
            {
                std::size_t new_capacity = get_next_capacity_or(size(), new_size);
                reallocate_storage(new_capacity, size());
            }
            // and populate vector with copy of value, or with value-initialization
            if constexpr (sizeof...(U) == 1)
                std::uninitialized_fill(end_, begin_ + new_size, value...);
            else
                std::uninitialized_value_construct(end_, begin_ + new_size);
            end_ = begin_ + new_size;
        }

        iterator destroy(iterator first, iterator last) noexcept
        {
            // optimization: don't call destructors for trivial types
            if constexpr(!std::is_trivial_v<value_type>)
            {
                for (auto it{first}; it < last; ++it)
                {
                    allocator_traits::destroy(alloc_, it);
                }
            }
            return first;
        }

        void release_storage() noexcept
        {
            if (begin_) // don't deallocate nullptr
            {
                destroy(begin_, end_);
                // std::allocator requires that pointer should be previously allocated by 'allocate',
                // despite the fact that operator delete is ok with nullptr
                allocator_traits::deallocate(alloc_, begin_, capacity());
                begin_ = nullptr;
                end_ = nullptr;
                capacity_ = nullptr;
            }
        }

        static constexpr std::size_t get_next_capacity_or(
            std::size_t current_size, std::size_t required_size = 0) noexcept
        {
            // basically, vector growths as power of 2 from current size,
            // but if required capacity is too much - returns it
            std::size_t new_capacity{current_size ? current_size * 2 : 1};
            return required_size > new_capacity ? required_size : new_capacity;
        }

        void reallocate_storage(std::size_t new_capacity, std::size_t current_size)
        {
            // allocate new storage
            auto new_memory = allocator_traits::allocate(alloc_, new_capacity);
            // move old data in new storage
            uninitialized_move_if_noexcept(begin_, begin_ + current_size, new_memory);
            // freed old storage
            release_storage();

            begin_ = new_memory;
            end_ = new_memory + current_size;
            capacity_ = new_memory + new_capacity;
        }

        void move_assign_impl(vector &&r)
        {
            release_storage();
            // propagate allocator if specified
            if constexpr (allocator_traits::propagate_on_container_move_assignment::value)
            {
                alloc_ = std::move(r.alloc_);
            }
            steal_pointers(std::move(r));
        }

        void steal_pointers(vector &&r) noexcept
        {
            begin_ = r.begin_;
            end_ = r.end_;
            capacity_ = r.capacity_;

            r.begin_ = nullptr;
            r.end_ = nullptr;
            r.capacity_ = nullptr;
        }

        pointer begin_{nullptr};
        pointer end_{nullptr};
        pointer capacity_{nullptr};

        // usually allocator is stateless, thus we don't occupy extra space for it
        [[no_unique_address]] allocator_type alloc_;
    };

    template <typename T, typename A>
    bool operator ==(const vector<T, A> &l, const vector<T, A> &r) noexcept
    {
        auto lit = l.cbegin();
        auto rit = r.cbegin();
        while (lit != l.cend() && rit != r.cend())
        {
            if(*lit++ != *rit++)
                return false;
        }
        return lit == l.cend() && rit == r.cend();
    }

    template <typename T, typename A>
    bool operator !=(const vector<T, A> &l, const vector<T, A> &r) noexcept
    {
        return !(l == r);
    }
}
