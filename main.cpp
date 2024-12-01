#include <iostream>
#include "TIntrusivePtr.h"

class TDoc : public TRefCounter<TDoc>
{
public:
    void Print() const
    {
        std::cout << "Document instance" << std::endl;
    }
};

int main()
{
    TIntrusivePtr<TDoc> ptr = nullptr;
    ptr = MakeIntrusive<TDoc>();
    std::cout << "After ptr contruction \n";
    std::cout << "prt.UseCount() =  " << ptr.UseCount() << std::endl;
    std::cout << std::endl;

    TIntrusivePtr<TDoc> ptr2 = ptr;
    std::cout << "After ptr2 = ptr \n";
    std::cout << "prt.UseCount() =  " << ptr.UseCount() << std::endl;
    std::cout << std::endl;

    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();

    ptr3.Reset(ptr2);
    std::cout << "After ptr3.Reset(ptr2) \n";
    std::cout << "prt.UseCount() =  " << ptr.UseCount() << std::endl;
    std::cout << "prt3.UseCount() =  " << ptr3.UseCount() << std::endl;
    std::cout << std::endl;

    ptr3.Reset();
    std::cout << "After ptr3.Reset() \n";
    std::cout << "prt.UseCount() =  " << ptr.UseCount() << std::endl;
    std::cout << "prt3.UseCount() =  " << ptr3.UseCount() << std::endl;
    std::cout << std::endl;

    ptr3.Reset(std::move(ptr2));
    std::cout << "After ptr3.Reset(std::move(ptr2)) \n";
    std::cout << "prt.UseCount() =  " << ptr.UseCount() << std::endl;
    std::cout << "prt2.UseCount() =  " << ptr2.UseCount() << std::endl;
    std::cout << "prt3.UseCount() =  " << ptr3.UseCount() << std::endl;
    std::cout << std::endl;

    static_assert(sizeof(TDoc*) == sizeof(ptr), "Size mismatch");
}
