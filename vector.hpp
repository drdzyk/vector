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

        using alloc_type = typename Alloc::template rebind<T>::other;
        using alloc_traits = std::allocator_traits<alloc_type>;

        template <typename ...Args>
        void emplace_back(Args&& ...args)
        {
            alloc_type alloc;
            if (end_ == capacity_)
            {
                reallocate_storage(alloc);
            }
            alloc_traits::construct(alloc, end_, std::forward<Args>(args)...);
            end_ += 1u;
        }

    private:
        void reallocate_storage(const alloc_type &alloc)
        {
            std::size_t how_many_stored = end_ - begin_;
            std::size_t next_capacity = get_next_capacity(how_many_stored);
            auto new_memory = alloc_traits::allocate(alloc, next_capacity);
            std::memcpy(new_memory, begin_, how_many_stored * sizeof(value_type));

            begin_ = new_memory;
            end_ = new_memory;
            capacity_ = new_memory + next_capacity;
        }
        static std::size_t get_next_capacity(std::size_t capacity) noexcept
        {
            return capacity ? capacity * 2 : 1;
        }

        pointer begin_{nullptr};
        pointer end_{nullptr};
        pointer capacity_{nullptr};
    };
}
