#include <iostream>

int Test1( int num )
{
    if( num > 50 )
    {
        throw std::exception();
    }
    if( num > 10 )
    {
        throw std::out_of_range("Test1 exception - OOR");
    }
    else if( num > 5 ) {
        throw std::runtime_error("Test1 exception - RTE");
    }
    
    return num;
}

void RunTest1(int num)
{
    try
    {
        std::cout << num << " results in: " << Test1(num) << std::endl;
    }
//    catch(const std::exception& e)  // would catch exceptions of any type
//    {
//        std::cerr << "caught in std::exception: " << e.what() << '\n';
//    }
    catch(std::runtime_error &e)
    {
        std::cerr << "runtime error: " << e.what() << '\n';
        // re-throw example, e.g. if we decide we can't handle afterall
        std::runtime_error modded("I'm afraid the caller will have to handle this one");
        e = modded;     // As e is reference, we replace the orig exception
        throw;
    }
    catch(std::out_of_range &e)
    {
        std::cerr << "out of range error: " << e.what() << '\n';
    }
    catch(...)
    {
        std::cerr << "caught in 'catch all' - no details available " << '\n';
    }
}

int Test2(int x) noexcept(false); // noexcept specifier
int Test2(int x) noexcept(false)
{
    // never throws
    ++x;
    return x;
}
int Test3(int z) noexcept(noexcept(Test2(z))); // noexcept specifier AND no except operator
int Test3(int x) noexcept(noexcept(Test2(x)))
{
    // compiler only adds unwind code to me if Test2 can throw
    // which can be determined via the noexcept operator
    std::cout << "Test 2 is declared as noexcept(" << std::boolalpha << noexcept(Test2(x)) <<")" << std::endl;
    return Test2(x);
}

int main()
{
    std::cout << "Starting..." << std::endl;

    std::cout << "Enter a number: ";

    int num = 0;
    std::cin >> num;

    RunTest1(num);

    Test3(5);
    
    std::cout << "Finished." << std::endl;

    return 0;
}