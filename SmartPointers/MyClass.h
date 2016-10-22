#include <iostream>

class MyClass
{
    public:

        MyClass();
        MyClass( char *somePtr_ );
        MyClass( const char *somePtr_ );
        ~MyClass(); 

        MyClass( const MyClass & );

        void Dump( void ) const;

        MyClass( MyClass&& ) noexcept;

        // Copy Assignment 
        MyClass& operator=( const MyClass & ); 

        // Move assignment operator
        MyClass& operator=( MyClass&& ) noexcept; 

        // Plus(+) Operator
        MyClass operator +( const MyClass &other ) const;

        // Prefix ++
        MyClass& operator++();

        // Postfix ++ (required dummy int arg to distinguish from prefix ++!)
        MyClass operator++(int dummyAlwaysZero);

        // Comparison Operator
        bool operator==(const MyClass &other ) const;

        // Function call operator
        void operator()();

        // Insertion & extraction operators are global functions as they
        // work with ostream/istream, but we need to make them friends of
        // this class to allow the functions access to the class members
        friend std::ostream & operator<<( std::ostream &out, const MyClass &rhs );
        friend std::istream & operator>>( std::istream &in, MyClass &rhs );

    private:
        char *m_SomePtr = nullptr;
};