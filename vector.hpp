#include <memory>
#include <cstring>

namespace low
{
    template <typename T, typename Alloc = std::allocator<T>>
    class vector
    {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        using alloc_type = typename Alloc::template rebind<T>::other;
        using alloc_traits = std::allocator_traits<alloc_type>;

        using iterator = __gnu_cxx::__normal_iterator<pointer, vector>;
        using const_iterator = __gnu_cxx::__normal_iterator<const_pointer, vector>;

        vector() = default;

        vector(vector &&r) noexcept :
            begin_(r.begin_),
            end_(r.end_),
            capacity_(r.capacity_)
        {
            r.begin_ = nullptr;
            r.end_ = nullptr;
            r.capacity_ = nullptr;
        }

        vector &operator=(vector &&r) noexcept
        {
            if (this != &r) // be on a safe side
            {
                begin_ = r.begin_;
                end_ = r.end_;
                capacity_ = r.capacity_;
                r.begin_ = nullptr;
                r.end_ = nullptr;
                r.capacity_ = nullptr;
            }

            return *this;
        }

        template <typename ...Args>
        void emplace_back(Args&& ...args)
        {
            alloc_type alloc;
            reallocate_storage_if_needed(alloc);
            alloc_traits::construct(alloc, end_, std::forward<Args>(args)...);
            end_ += 1u;
        }

        void reserve(std::size_t new_capacity)
        {
            if (new_capacity > capacity())
            {
                alloc_type alloc;
                reallocate_storage(alloc, new_capacity, size());
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

        std::size_t size() const noexcept { return end_ - begin_; }
        std::size_t capacity() const noexcept { return capacity_ - begin_; }

        iterator begin() noexcept { return iterator{begin_}; }
        iterator end() noexcept { return iterator{end_}; }

        const_iterator cbegin() const noexcept { return const_iterator{begin_}; }
        const_iterator cend() const noexcept { return const_iterator{end_}; }

        ~vector() noexcept
        {
            alloc_type alloc;
            release_storage(alloc);
        }

    private:
        template <typename ...U>
        void resize_impl(std::size_t new_size, const U& ...value)
        {
            static_assert(sizeof...(U) <= 1);
            static_assert((std::is_same_v<U, value_type> && ...),
                    "if 'value' present, then it should be of 'value_type' type");

            alloc_type alloc;
            const auto diff = static_cast<std::ptrdiff_t>(new_size) - static_cast<std::ptrdiff_t>(size());
            if (diff < 0)
            {
                for (std::ptrdiff_t count{diff}; count < 0; ++count)
                {
                    end_ -= 1u;
                    alloc_traits::destroy(alloc, end_);
                }
            }
            else if (diff > 0)
            {
                if (capacity() < new_size)
                {
                    std::size_t current_size{size()};
                    std::size_t new_capacity{current_size ? current_size * 2 : 1};
                    reallocate_storage(alloc,
                       new_size > new_capacity ? new_size : new_capacity,
                        current_size);
                }
                for (std::ptrdiff_t count{0}; count < diff; ++count)
                {
                    alloc_traits::construct(alloc, end_, value...);
                    end_ += 1u;
                }
            }
        }

        void release_storage(alloc_type &alloc) noexcept
        {
            for (auto it = begin(); it != end(); ++it)
            {
                alloc_traits::destroy(alloc, it.base());
            }
            alloc_traits::deallocate(alloc, begin_, capacity());
        }

        void reallocate_storage(alloc_type &alloc, std::size_t new_capacity, std::size_t new_size)
        {
            // allocate new storage
            auto new_memory = alloc_traits::allocate(alloc, new_capacity);

            // move old data in new storage
            if constexpr (std::is_nothrow_move_constructible_v<value_type>)
                std::uninitialized_move(begin_, begin_ + new_size, new_memory);
            else
                std::uninitialized_copy(begin_, begin_ + new_size, new_memory);

            // freed old storage
            release_storage(alloc);

            begin_ = new_memory;
            end_ = new_memory + new_size;
            capacity_ = new_memory + new_capacity;
        }

        void reallocate_storage_if_needed(alloc_type &alloc)
        {
            if (auto current_size = size(); current_size == capacity())
            {
                std::size_t new_capacity{current_size ? current_size * 2 : 1};
                reallocate_storage(alloc, new_capacity, current_size);
            }
        }

        pointer begin_{nullptr};
        pointer end_{nullptr};
        pointer capacity_{nullptr};
    };
}
