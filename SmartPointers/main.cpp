#include <iostream>

#include <memory>   // smart pointer types

#include "MyClass.h"

void TryOut_UniquePtr(void);
void UniquePtrMoveTest( std::unique_ptr<MyClass> ptr );
void UniquePtrReferenceTest( std::unique_ptr<MyClass> &ptr );

void TryOut_SharedPtr(void);
void SharedPtrCopyTest( std::shared_ptr<MyClass> ptr );

void TryOut_WeakPtr(void);

int main()
{
   // TryOut_UniquePtr();
    // TryOut_SharedPtr();
    TryOut_WeakPtr();
    return 0;
}

void TryOut_UniquePtr()
{
    // Unique Pointer:
    //
    std::unique_ptr<MyClass> up( new MyClass("uniqueptrTest") );
    up->Dump();
    (*up).Dump();

    // UniquePtrMoveTest(up);   // passing by value would create a copy,
                                // so fails with: error: use of deleted function

    UniquePtrReferenceTest( up );   // Passs by reference is OK
    up->Dump();

    UniquePtrMoveTest( std::move(up) );
    //up->Dump();               // segmentation fault - destroyed already

    // no need to delete unique ptr
}
void UniquePtrMoveTest( std::unique_ptr<MyClass> ptr )
{
    ptr->Dump();
}

void UniquePtrReferenceTest( std::unique_ptr<MyClass> &ptr )
{
    ptr->Dump();
}

void TryOut_SharedPtr()
{
    // Shared Pointer:
    //
    std::shared_ptr<MyClass> sp( new MyClass("sharedptrTest") );
    sp->Dump();
    (*sp).Dump();

    SharedPtrCopyTest( sp );
    sp->Dump();

    // no need to delete shared ptr
}
void SharedPtrCopyTest( std::shared_ptr<MyClass> ptr )
{
    ptr->Dump();
}

void TryOut_WeakPtr()
{
    class Printer {
        std::weak_ptr<MyClass> m_WeakPtr;
        public:
             void SetValue( std::weak_ptr<MyClass> weakPtr) {
                 m_WeakPtr = weakPtr;
             }
             void Print() {
                 if( !m_WeakPtr.expired() ) {
                     // lock to create our own shared_ptr
                     std::shared_ptr<MyClass> sharedPtr = m_WeakPtr.lock();
                     sharedPtr->Dump();
                 }
             }
    };

    Printer myPrinter;
    std::shared_ptr<MyClass> deleteMe(new MyClass("Delete"));
    myPrinter.SetValue(deleteMe);
    if( *deleteMe == "Delete")
    {
        // delete mem outside of the class...
        deleteMe = nullptr;     // deletes the shared_ptr
    }
    myPrinter.Print();  // OK - doesn't crash as print checks!
}