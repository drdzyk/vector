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

        // employ user-provided allocator, since we ensured that it value_type is same as for vector,
        // thus no need to rebound allocator
        using allocator_type = Alloc;
        using allocator_traits = std::allocator_traits<Alloc>;

        using iterator = __gnu_cxx::__normal_iterator<pointer, vector>;
        using const_iterator = __gnu_cxx::__normal_iterator<const_pointer, vector>;

        vector() = default;

        explicit vector(const allocator_type &alloc) noexcept : meta_(alloc) {}

        vector(vector &&r) noexcept :
            meta_(std::move(r.meta_))
        {
            r.meta_.begin_ = nullptr;
            r.meta_.end_ = nullptr;
            r.meta_.capacity_ = nullptr;
        }

        vector(vector &&r, const allocator_type &alloc) :
            meta_(alloc)
        {
            if (r.get_allocator() != alloc) // exactly this comparison required by standard,
                // see https://en.cppreference.com/w/cpp/container/vector/vector
            {
                // perform element-wise move of stored objects
                reserve(r.size());
                meta_.end_ = std::uninitialized_move(r.meta_.begin_, r.meta_.end_, meta_.begin_);
            }
            else
            {
                // just steal a pointers
                meta_.begin_ = r.meta_.begin_;
                meta_.end_ = r.meta_.end_;
                meta_.capacity_ = r.meta_.capacity_;
                r.meta_.begin_ = nullptr;
                r.meta_.end_ = nullptr;
                r.meta_.capacity_ = nullptr;
            }
        }

        vector(const vector &r) :
            vector(r, allocator_traits::select_on_container_copy_construction(r.get_allocator()))
        {}

        vector(const vector &r, const allocator_type &alloc) :
            meta_(alloc)
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
            if constexpr (allocator_traits::propagate_on_container_move_assignment::value)
            {
                release_storage();
                meta_ = std::move(r.meta_);
                r.meta_.begin_ = nullptr;
                r.meta_.end_ = nullptr;
                r.meta_.capacity_ = nullptr;
                return *this;
            }
            if (get_allocator() == r.get_allocator())
            {
                release_storage();
                meta_.begin_ = r.meta_.begin_;
                meta_.end_ = r.meta_.end_;
                meta_.capacity_ = r.meta_.capacity_;
                r.meta_.begin_ = nullptr;
                r.meta_.end_ = nullptr;
                r.meta_.capacity_ = nullptr;
                return *this;
            }
            if (size() >= r.size())
            {
                auto end = std::move(r.meta_.begin_, r.meta_.end_, meta_.begin_);
                for (auto cur = end; cur != meta_.end_; ++cur)
                {
                    allocator_traits::destroy(meta_, cur);
                }
                meta_.end_ = end;
            }
            else
            {
                release_storage();
                reserve(r.size());
                meta_.end_ = std::uninitialized_move(r.meta_.begin_, r.meta_.end_, meta_.begin_);
            }

            return *this;
        }

        template <typename It>
        void assign(It first, It last)
        {
            const auto distance = static_cast<std::size_t>(std::distance(first, last));
            if (capacity() < distance)
            {
                release_storage();
                reserve(distance);
            }

            const It pivot = first + std::min(size(), distance);
            const pointer end = std::move(first, pivot, meta_.begin_);
            for (pointer it{end}; it != meta_.end_; ++it)
            {
                allocator_traits::destroy(meta_, it);
            }
            meta_.end_ = std::uninitialized_move(pivot, last, end);
        }

        template <typename ...Args>
        void emplace_back(Args&& ...args)
        {
            reallocate_storage_if_needed();
            allocator_traits::construct(meta_, meta_.end_, std::forward<Args>(args)...);
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

        std::size_t size() const noexcept { return meta_.end_ - meta_.begin_; }
        std::size_t capacity() const noexcept { return meta_.capacity_ - meta_.begin_; }
        bool empty() const noexcept { return size() == 0u; }

        iterator begin() noexcept { return iterator{meta_.begin_}; }
        iterator end() noexcept { return iterator{meta_.end_}; }

        const_iterator begin() const noexcept { return const_iterator{meta_.begin_}; }
        const_iterator end() const noexcept { return const_iterator{meta_.end_}; }

        const_iterator cbegin() const noexcept { return const_iterator{meta_.begin_}; }
        const_iterator cend() const noexcept { return const_iterator{meta_.end_}; }

        // meet AllocatorAwareContainer requirements;
        const allocator_type &get_allocator() const noexcept { return meta_; }

        void clear() noexcept
        {
            for (auto it = begin(); it != end(); ++it)
            {
                allocator_traits::destroy(meta_, it.base());
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
                    allocator_traits::destroy(meta_, meta_.end_);
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
                    allocator_traits::construct(meta_, meta_.end_, value...);
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
                allocator_traits::deallocate(meta_, meta_.begin_, capacity());
                meta_.begin_ = nullptr;
                meta_.end_ = nullptr;
                meta_.capacity_ = nullptr;
            }
        }

        void reallocate_storage(std::size_t new_capacity, std::size_t new_size)
        {
            // allocate new storage
            auto new_memory = allocator_traits::allocate(meta_, new_capacity);

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

        // this hack with EBO(empty base optimization) allow to reduce sizeof vector when allocator is stateless
        struct meta : allocator_type
        {
            meta() = default;
            explicit meta(const allocator_type &alloc) noexcept : allocator_type(alloc) {}

            pointer begin_{nullptr};
            pointer end_{nullptr};
            pointer capacity_{nullptr};
        } meta_;
    };
}
