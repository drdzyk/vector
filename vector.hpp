#include <memory>
#include <cstring>

namespace low
{
    template <typename T, typename Alloc = std::allocator<T>>
    class vector
    {
        static_assert(std::is_pod_v<T>); // WIP

    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using const_reference = const T&;

        using alloc_type = typename Alloc::template rebind<T>::other;
        using alloc_traits = std::allocator_traits<alloc_type>;

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
            std::memcpy(new_memory, begin_, current_size * sizeof(value_type));
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
