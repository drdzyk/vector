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

        template <typename ...Args>
        void emplace_back(Args&& ...args)
        {
            alloc_type alloc;
            if (size() == capacity())
            {
                reallocate_storage(alloc);
            }
            alloc_traits::construct(alloc, end_, std::forward<Args>(args)...);
            end_ += 1u;
        }

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
            alloc_traits::deallocate(alloc, begin_, capacity_ - begin_);
        }

    private:
        void reallocate_storage(alloc_type &alloc)
        {
            std::size_t current_size = size();
            std::size_t next_capacity = get_next_capacity(current_size);

            // allocate new storage
            auto new_memory = alloc_traits::allocate(alloc, next_capacity);
            // store holded data in new storage
            std::copy(begin_, end_, new_memory);
            // freed old storage
            alloc_traits::deallocate(alloc, begin_, current_size);

            begin_ = new_memory;
            end_ = new_memory + current_size;
            capacity_ = new_memory + next_capacity;
        }
        static std::size_t get_next_capacity(std::size_t size) noexcept { return size ? size * 2 : 1; }

        pointer begin_{nullptr};
        pointer end_{nullptr};
        pointer capacity_{nullptr};
    };
}
