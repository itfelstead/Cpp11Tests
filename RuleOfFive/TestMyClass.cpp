#include "MyClass.h"

#include <iostream>
#include <string.h>

MyClass TestCallsMadeVal( MyClass byVal )
{
    std::cout << "Fn TestCallsMadeVal() called" << std::endl;

    byVal.Dump();

    std::cout << "Returning new one..." << std::endl;

    return MyClass( strdup("returned") );
}

MyClass TestCallsMadeRef( MyClass &byRef )
{
    std::cout << "Fn TestCallsMadeRef() called" << std::endl;

    byRef.Dump();

    std::cout << "Returning new one..." << std::endl;

    return MyClass( strdup("returned") );
}

void TestCallOrder()
{
    std::cout << "Start TestCallOrder------------------------" << std::endl;

    MyClass base{ strdup("base") };

    // MyClass ret = TestCallsMadeRef( base );

    MyClass ret = TestCallsMadeVal( base );

    std::cout << "End TestCallOrder------------------------" << std::endl;
}

int main()
{
    std::cout << "Starting MyClass test..." << std::endl;
TestCallOrder();
return 0;
    MyClass a{ strdup("valid value") };     // Param CTOR called.

    MyClass b = a;  // Copy CTOR called. now we have two copies; a & b
    b.Dump();                   // prints "valid value"
    a.Dump();                   // prints "valid value"

    MyClass c = std::move(b);   // Move CTOR called. still 2 copies, a & c - b is uninitialised
    b.Dump();                   // prints nullptr (it is uninitialised)
    c.Dump();                   // prints "valid value"
    a.Dump();                   // prints "valid value"
    
    MyClass d{ strdup("try this") };        // Param CTOR called.
    d = std::move(c);           // Move Assignment called
    d.Dump();                   // prints "valid value"
    c.Dump();                   // prints nullptr (unless we
                                // implemented it with std::swap()
                                // in which case it would be "try this"

    std::cout << "Done MyClass test..." << std::endl;

    return 0;
}

