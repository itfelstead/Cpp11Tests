#include "MyClass.h"

#include <iostream>
#include <string.h>

const MyClass TryCopyElision()
{
    MyClass obj{ strdup("return by copy") };
    obj.Dump();

    // We can't return a ref as it is a local object, so we return
    // by value (copy) and compiler copy elision (unless disabled)
    // will ensure efficiency
    return obj;
}

int main()
{
    std::cout << "Starting MyClass test..." << std::endl;

    MyClass myCopy = TryCopyElision();
    myCopy.Dump();

    std::cout << "Done MyClass test..." << std::endl;

    return 0;
}