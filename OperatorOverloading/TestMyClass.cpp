#include "MyClass.h"

#include <iostream>
#include <string.h>

std::ostream & operator<<( std::ostream &out, const MyClass &rhs )
{
    out << rhs.m_SomePtr; 
    //rhs.Dump(); 
    return out;
}

std::istream & operator>>( std::istream &in, MyClass &rhs )
{
    std::string tmp;
    in >> tmp;
    rhs = tmp.c_str();

    return in;
}

int main()
{
    std::cout << "Starting MyClass test..." << std::endl;


    MyClass a{ strdup("A") };
    MyClass b{ strdup("B") };
    MyClass sum = a + b;
    sum.Dump();

    ++a;
    a.Dump();

    a++;
    a.Dump();

    std::cout << "comparison a==b: " << ((a==b)?"SAME":"NOT SAME") << std::endl; 
    MyClass c;
    c = b;
    std::cout << "comparison c==b: " << ((b==c)?"SAME":"NOT SAME") << std::endl; 
  
    const char *myCstring = "implicit";
    MyClass implicitTest = c + (const char *)myCstring;
    implicitTest.Dump();

    std::cout << "Trying operator<<: " << implicitTest << std::endl;

    std::cin >> c;
    std::cout << "C is now " << c << std::endl;

    std::cout << "calling function operator on c: " << std::endl;
    c();

    std::cout << "Done MyClass test..." << std::endl;

    MyClass zzz;
    
    zzz = 69;

    return 0;
}