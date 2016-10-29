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

MyClass::MyClass( const char *m_SomePtr_ )
{
    std::cout << "Param CTOR - MyClass::MyClass( const char *m_SomePtr_ )" << std::endl;
    if( m_SomePtr_ != nullptr )
    {
        m_SomePtr = strdup( m_SomePtr_ );
    }
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



// Move Constructor
MyClass::MyClass( MyClass&& toMove ) noexcept
{
    std::cout << "Move CTOR - MyClass::MyClass( MyClass&& toMove )" << std::endl;

    // same as m_SomePtr = toMove.m_SomePtr; toMove.m_SomePtr=nullptr;
    m_SomePtr = std::exchange( toMove.m_SomePtr, nullptr);
}          


void MyClass::Dump( void ) const
{
    std::cout << "MyClass::Dump(): " << (m_SomePtr==nullptr?"NULL":m_SomePtr) << std::endl;
}

// Copy Assignment
MyClass& MyClass::operator=( const MyClass &rhs )
{
    std::cout << "Copy Assignment - MyClass& MyClass::operator=( const MyClass &toCopy )" << std::endl;
    if( this != &rhs )
    {
        free( m_SomePtr );

        if( rhs.m_SomePtr == nullptr )
        {
            m_SomePtr = nullptr;
        }
        else
        {
            m_SomePtr = strdup( rhs.m_SomePtr );
        }
    }   
    return *this;
}

// Move Assignment Operator
MyClass& MyClass::operator=( MyClass&& rhs ) noexcept
{
    std::cout << "Move Assignment - MyClass& MyClass::operator=( MyClass&& toMove )" << std::endl;

    if( this != &rhs )
    {
        // Some folk might use std::swap() to ensure function is noexcept,
        // at the expense of delaying destruction of original value..
        //      std::swap( m_SomePtr, toMove.m_SomePtr );
        // Here I'm going with a straight forward destroy/release
        free( m_SomePtr );
        m_SomePtr = rhs.m_SomePtr;
        rhs.m_SomePtr = nullptr;
    }
    return *this;
}

MyClass MyClass::operator+( const char *rhs ) const
{
    MyClass result( strdup(rhs) );
    return result;     
}

MyClass MyClass::operator+( const MyClass &other ) const
{
    std::string work = m_SomePtr;
    work = work + other.m_SomePtr;

    MyClass result( strdup(work.c_str()) );
    return result;
}

// Prefix ++ Operator
MyClass& MyClass::operator++()
{
    // err... just append "++" ;-)
    std::string work{ m_SomePtr };
    work += "++";
    free( m_SomePtr );
    m_SomePtr = strdup( work.c_str() );
    return *this;   // Note: returns by ref (as not a local var)
}

// Postfix ++ Operator (less efficient as requires temp object)
MyClass MyClass::operator++( int dummyAlwaysZero )
{
    MyClass orig( *this );
    ++(*this);
    return orig;    // Note: copy, not ref
}

bool MyClass::operator==(const MyClass &other ) const
{
    if( m_SomePtr == nullptr || other.m_SomePtr == nullptr )
    {
        return m_SomePtr==other.m_SomePtr;
    }
    else
    {
        return strcmp(m_SomePtr, other.m_SomePtr)==0;
    }
}

void MyClass::operator()()
{
    // Do whatever
    Dump();
}


//MyClass::operator int()