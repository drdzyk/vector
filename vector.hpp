#pragma once

#include <algorithm>
#include <memory>

namespace low
{
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
            alloc_(std::move(r.alloc_)),
            meta_(std::move(r.meta_))
        {}

        vector(vector &&r, const allocator_type &alloc)
            noexcept(allocator_traits::is_always_equal::value) : alloc_(alloc)
        {
            // compile time knowledge, that allocators always equal
            if constexpr (allocator_traits::is_always_equal::value)
            {
                // if so, just steal a pointers
                meta_.steal_pointers(std::move(r.meta_));
                return;
            }
            // otherwise perform runtime dispatch
            if (r.get_allocator() == alloc)
            {
                // just steal a pointers
                meta_.steal_pointers(std::move(r.meta_));
            }
            else
            {
                // perform element-wise move of stored objects
                reserve(r.size());
                meta_.end_ = std::uninitialized_move(r.meta_.begin_, r.meta_.end_, meta_.begin_);
            }
        }

        vector(const vector &r) :
            vector(r, allocator_traits::select_on_container_copy_construction(r.get_allocator()))
        {}

        vector(const vector &r, const allocator_type &alloc) :
            alloc_(alloc)
        {
            reserve(r.size());
            meta_.end_ = std::uninitialized_copy(r.meta_.begin_, r.meta_.end_, meta_.begin_);
        }

        vector(const std::initializer_list<value_type> &list)
        {
            reserve(list.size());
            meta_.end_ = std::uninitialized_copy(list.begin(), list.end(), meta_.begin_);
        }

        vector &operator=(vector &&r) noexcept
        {
            if (this == &r) // be on a safe side
            {
                return *this;
            }
            // compile time knowledge, that we need to steal resources from other and propagate allocator
            if constexpr (allocator_traits::propagate_on_container_move_assignment::value)
            {
                release_storage();
                alloc_ = std::move(r.alloc_);
                meta_ = std::move(r.meta_);
                return *this;
            }
            // if propagation prohibited, perform runtime dispatch:
            if (get_allocator() == r.get_allocator())
            {
                // if allocators equal, just steal resources
                release_storage();
                meta_.steal_pointers(std::move(r.meta_));
            }
            else
            {
                // otherwise, perform element-wise move
                assign(std::move_iterator{r.begin()}, std::move_iterator{r.end()});
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
            const pointer end = std::copy(first, pivot, meta_.begin_);

            // destruct old remaining elements if there are any
            for (pointer it{end}; it != meta_.end_; ++it)
            {
                allocator_traits::destroy(alloc_, it);
            }

            // copy-construct the rest elements from initial [first, last)
            // range if there are any
            meta_.end_ = std::uninitialized_copy(pivot, last, end);
        }

        template <typename ...Args>
        void emplace_back(Args&& ...args)
        {
            reallocate_storage_if_needed();
            allocator_traits::construct(alloc_, meta_.end_, std::forward<Args>(args)...);
            meta_.end_ += 1u;
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
            return *(meta_.begin_ + idx);
        }

        const_reference operator[](std::size_t idx) const
        {
            return *(meta_.begin_ + idx);
        }

        std::size_t size() const noexcept { return static_cast<std::size_t>(meta_.end_ - meta_.begin_); }
        std::size_t capacity() const noexcept { return static_cast<std::size_t>(meta_.capacity_ - meta_.begin_); }
        bool empty() const noexcept { return size() == 0u; }

        iterator begin() noexcept { return meta_.begin_; }
        iterator end() noexcept { return meta_.end_; }

        const_iterator begin() const noexcept { return meta_.begin_; }
        const_iterator end() const noexcept { return meta_.end_; }

        const_iterator cbegin() const noexcept { return meta_.begin_; }
        const_iterator cend() const noexcept { return meta_.end_; }

        const allocator_type &get_allocator() const noexcept { return alloc_; }

        void clear() noexcept
        {
            for (auto it = begin(); it != end(); ++it)
            {
                allocator_traits::destroy(alloc_, it);
            }
            meta_.end_ = meta_.begin_;
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
        template <typename ...U>
        void resize_impl(std::size_t new_size, const U& ...value)
        {
            static_assert(sizeof...(U) <= 1);
            static_assert((std::is_same_v<U, value_type> && ...),
                    "if 'value' present, then it should be of 'value_type' type");

            const auto diff = static_cast<std::ptrdiff_t>(new_size) - static_cast<std::ptrdiff_t>(size());
            if (diff < 0)
            {
                for (std::ptrdiff_t count{diff}; count < 0; ++count)
                {
                    meta_.end_ -= 1u;
                    allocator_traits::destroy(alloc_, meta_.end_);
                }
            }
            else if (diff > 0)
            {
                if (capacity() < new_size)
                {
                    std::size_t current_size{size()};
                    std::size_t new_capacity{current_size ? current_size * 2 : 1};
                    reallocate_storage(
                       new_size > new_capacity ? new_size : new_capacity,
                        current_size);
                }
                for (std::ptrdiff_t count{0}; count < diff; ++count)
                {
                    allocator_traits::construct(alloc_, meta_.end_, value...);
                    meta_.end_ += 1u;
                }
            }
        }

        void release_storage() noexcept
        {
            clear();
            if (meta_.begin_) // don't deallocate nullptr
            {
                // std::allocator requires that pointer should be previously allocated by 'allocate',
                // despite the fact that operator delete is ok with nullptr
                allocator_traits::deallocate(alloc_, meta_.begin_, capacity());
                meta_.begin_ = nullptr;
                meta_.end_ = nullptr;
                meta_.capacity_ = nullptr;
            }
        }

        void reallocate_storage(std::size_t new_capacity, std::size_t new_size)
        {
            // allocate new storage
            auto new_memory = allocator_traits::allocate(alloc_, new_capacity);

            // move old data in new storage
            if constexpr (std::is_nothrow_move_constructible_v<value_type>)
                std::uninitialized_move(meta_.begin_, meta_.begin_ + new_size, new_memory);
            else
                std::uninitialized_copy(meta_.begin_, meta_.begin_ + new_size, new_memory);

            // freed old storage
            release_storage();

            meta_.begin_ = new_memory;
            meta_.end_ = new_memory + new_size;
            meta_.capacity_ = new_memory + new_capacity;
        }

        void reallocate_storage_if_needed()
        {
            if (auto current_size = size(); current_size == capacity())
            {
                std::size_t new_capacity{current_size ? current_size * 2 : 1};
                reallocate_storage(new_capacity, current_size);
            }
        }

        [[no_unique_address]] allocator_type alloc_;
        struct meta
        {
            meta() = default;
            meta(meta &&r) noexcept
            {
                steal_pointers(std::move(r));
            }
            meta &operator=(meta &&r) noexcept
            {
                steal_pointers(std::move(r));
                return *this;
            }
            meta &operator=(const meta &) noexcept
            {
                return *this;
            }
            meta(const meta &) = delete;
            ~meta() = default;

            void steal_pointers(meta &&other) noexcept
            {
                begin_ = other.begin_;
                end_ = other.end_;
                capacity_ = other.capacity_;
                other.begin_ = nullptr;
                other.end_ = nullptr;
                other.capacity_ = nullptr;
            }

            pointer begin_{nullptr};
            pointer end_{nullptr};
            pointer capacity_{nullptr};
        } meta_;
    };
}
