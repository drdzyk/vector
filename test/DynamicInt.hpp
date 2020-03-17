#pragma once

class DynamicInt
{
public:
    DynamicInt() = default;
    DynamicInt(int i) : i_(new int(i)) {}
    DynamicInt(const DynamicInt &other) : i_(other.i_ ? new int(*other.i_) : nullptr)
    {
    }
    DynamicInt(DynamicInt &&other): i_(other.i_)
    {
        other.i_ = nullptr;
    }
    DynamicInt& operator=(const DynamicInt &other)
    {
        delete i_;
        i_ = new int(*other.i_);
        return *this;
    }
    DynamicInt& operator=(DynamicInt &&other)
    {
        delete i_;
        i_ = other.i_;
        other.i_ = nullptr;
        return *this;
    }

    ~DynamicInt() { delete i_; }

    friend bool operator==(const DynamicInt &lhs, const DynamicInt &rhs) noexcept
    {
        if (rhs.i_ == nullptr && lhs.i_ == nullptr)
        {
            return true;
        }
        if (rhs.i_ == nullptr || lhs.i_ == nullptr)
        {
            return false;
        }
        return *lhs.i_ == *rhs.i_;
    }
private:
    int *i_{nullptr};
};
