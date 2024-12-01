#pragma once
#include <utility>

template <typename T>
class TRefCounter 
{
    size_t RefCount_;

public:
    TRefCounter() : RefCount_(0) {}

    void AddRef() 
    {
        ++RefCount_;
    }

    void ReleaseRef() {
        if (--RefCount_ == 0) 
        {
            delete static_cast<T*>(this);
        }
    }

    size_t RefCount() const 
    {
        return RefCount_;
    }
};

template <typename T>
class TIntrusivePtr 
{
    T* Ptr_;

public:
    TIntrusivePtr() : Ptr_(nullptr) {}
    TIntrusivePtr(std::nullptr_t) : Ptr_(nullptr) {}


    explicit TIntrusivePtr(T* ptr) : Ptr_(ptr) 
    {
        if (Ptr_)
            Ptr_->AddRef();
    }

    TIntrusivePtr(const TIntrusivePtr& other) : Ptr_(other.Ptr_) 
    {
        if (Ptr_) 
            Ptr_->AddRef();
    }

    TIntrusivePtr(TIntrusivePtr&& other) noexcept : Ptr_(other.Ptr_) 
    {
        other.Ptr_ = nullptr;
    }

    ~TIntrusivePtr() 
    {
        if (Ptr_)
            Ptr_->ReleaseRef();
    }

    TIntrusivePtr& operator=(const TIntrusivePtr& other) 
    {
        if (this != &other) 
        {
            Reset();
            Ptr_ = other.Ptr_;
            if (Ptr_) 
                Ptr_->AddRef();
        }
        return *this;
    }

    TIntrusivePtr& operator=(TIntrusivePtr&& other) noexcept 
    {
        if (this != &other) 
        {
            Reset();
            Ptr_ = other.Ptr_;
            other.Ptr_ = nullptr;
        }
        return *this;
    }

    void Reset(T* ptr = nullptr) 
    {
        if (Ptr_)
            Ptr_->ReleaseRef();

        Ptr_ = ptr;
        if (Ptr_) 
            Ptr_->AddRef();
    }

    void Reset(TIntrusivePtr&& other) noexcept 
    {
        Reset();
        Ptr_ = other.Ptr_;
        other.Ptr_ = nullptr;
    }

    void Reset(const TIntrusivePtr& other) 
    {
        if (Ptr_ != other.Ptr_) 
        {
            if (Ptr_) 
                Ptr_->ReleaseRef();
            
            Ptr_ = other.Ptr_;
            if (Ptr_) 
                Ptr_->AddRef();
        }
    }

    T* Get() const 
    {
        return Ptr_;
    }

    size_t UseCount() const 
    {
        return Ptr_ ? Ptr_->RefCount() : 0;
    }

    explicit operator bool() const 
    {
        return Ptr_ != nullptr;
    }

    T* operator->() const 
    {
        return Ptr_;
    }

    T& operator*() const 
    {
        return *Ptr_;
    }

    bool operator==(const TIntrusivePtr& other) const 
    {
        return Ptr_ == other.Ptr_;
    }

    bool operator!=(const TIntrusivePtr& other) const 
    {
        return Ptr_ != other.Ptr_;
    }
};

// Функция MakeIntrusive
template <typename T, typename... Args>
TIntrusivePtr<T> MakeIntrusive(Args&&... args) 
{
    return TIntrusivePtr<T>(new T(std::forward<Args>(args)...));
}
