#include <memory>
#include <cstring>

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
            reallocate_storage_if_needed();
            allocator_traits::construct(alloc_, end_, std::forward<Args>(args)...);
            end_ += 1u;
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

        std::size_t size() const noexcept { return end_ - begin_; }
        std::size_t capacity() const noexcept { return capacity_ - begin_; }

        iterator begin() noexcept { return iterator{begin_}; }
        iterator end() noexcept { return iterator{end_}; }

        const_iterator cbegin() const noexcept { return const_iterator{begin_}; }
        const_iterator cend() const noexcept { return const_iterator{end_}; }

        ~vector() noexcept
        {
            release_storage();
        }

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
                    end_ -= 1u;
                    allocator_traits::destroy(alloc_, end_);
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
                    allocator_traits::construct(alloc_, end_, value...);
                    end_ += 1u;
                }
            }
        }

        void release_storage() noexcept
        {
            for (auto it = begin(); it != end(); ++it)
            {
                allocator_traits::destroy(alloc_, it.base());
            }
            allocator_traits::deallocate(alloc_, begin_, capacity());
        }

        void reallocate_storage(std::size_t new_capacity, std::size_t new_size)
        {
            // allocate new storage
            auto new_memory = allocator_traits::allocate(alloc_, new_capacity);

            // move old data in new storage
            if constexpr (std::is_nothrow_move_constructible_v<value_type>)
                std::uninitialized_move(begin_, begin_ + new_size, new_memory);
            else
                std::uninitialized_copy(begin_, begin_ + new_size, new_memory);

            // freed old storage
            release_storage();

            begin_ = new_memory;
            end_ = new_memory + new_size;
            capacity_ = new_memory + new_capacity;
        }

        void reallocate_storage_if_needed()
        {
            if (auto current_size = size(); current_size == capacity())
            {
                std::size_t new_capacity{current_size ? current_size * 2 : 1};
                reallocate_storage(new_capacity, current_size);
            }
        }

        allocator_type alloc_;
        pointer begin_{nullptr};
        pointer end_{nullptr};
        pointer capacity_{nullptr};
    };
}
