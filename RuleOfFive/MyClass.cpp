#include "MyClass.h"

#include <cstring>
#include <algorithm>

#include <iostream>

// Default CTOR
MyClass::MyClass( ) : m_SomePtr(NULL)
{
    std::cout << "CTOR - MyClass::MyClass( )" << std::endl;
}

// Parametised CTOR
MyClass::MyClass( char *m_SomePtr_ ) : m_SomePtr(m_SomePtr_)
{
    std::cout << "Param CTOR - MyClass::MyClass( char *m_SomePtr_ )" << std::endl;
}

// DTOR
MyClass::~MyClass()
{
    std::cout << "DTOR - MyClass::~MyClass( )" << std::endl;
    free( m_SomePtr );
}

// Copy Constructor
MyClass::MyClass( const MyClass &toCopy )
{
    std::cout << "Copy CTOR - MyClass::MyClass( const MyClass &toCopy )" << std::endl;

    if( toCopy.m_SomePtr != nullptr )
    {
        std::size_t numChars = std::strlen(toCopy.m_SomePtr) + 1;

        m_SomePtr = new char[numChars];
        std::memcpy( m_SomePtr, toCopy.m_SomePtr, numChars );
    }
}

// Copy Assignment
MyClass& MyClass::operator=( const MyClass &toCopy )
{
    std::cout << "Copy Assignment - MyClass& MyClass::operator=( const MyClass &toCopy )" << std::endl;

    return *this = MyClass(toCopy);
}

// Move Constructor
MyClass::MyClass( MyClass&& toMove ) noexcept
{
    std::cout << "Move CTOR - MyClass::MyClass( MyClass&& toMove )" << std::endl;

    // same as m_SomePtr = toMove.m_SomePtr; toMove.m_SomePtr=nullptr;
    m_SomePtr = std::exchange( toMove.m_SomePtr, nullptr);
}          

// Move Assignment Operator
MyClass& MyClass::operator=( MyClass&& toMove ) noexcept
{
    std::cout << "Move Assignment - MyClass& MyClass::operator=( MyClass&& toMove )" << std::endl;

    // Some folk might use std::swap() to ensure function is noexcept,
    // at the expense of delaying destruction of original value..
    //      std::swap( m_SomePtr, toMove.m_SomePtr );
    // Here I'm going with a straight forward destroy/release
    free( m_SomePtr );
    m_SomePtr = toMove.m_SomePtr;
    toMove.m_SomePtr = nullptr;
    return *this;
}

void MyClass::Dump( void ) const
{
    std::cout << "MyClass::Dump(): " << (m_SomePtr==nullptr?"NULL":m_SomePtr) << std::endl;
}