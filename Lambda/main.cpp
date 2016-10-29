#include <iostream>
 
// Note: compiler auto deduction of size from array reference
template<typename T, int size, typename TCallback> 
void ForEach( T(&myArray)[size], TCallback MyOperation )
{
    for( int i=0; i<size-1; ++i ) {
        MyOperation( myArray[i] );
    }
}

void TestCaptureList()
{
    int data[]{ 1,5,8,2,5,1};

    // Just print
    ForEach( data, [](auto arg) { 
        std::cout << "doing " << arg << std::endl;
     } );

    // Add a value using a captured offset (a COPY of offset)
    int offset = 10;
    ForEach( data, [offset](auto &arg) { 
        std::cout << "doing " << arg;
        arg = arg + offset;
        // Note: compiler error if we try to modify offset
        std::cout << ": " << arg << std::endl;
    } );

    // Allow offset to be modified inside the lambda via 'mutable'
    // (lambda's copy is modified, NOT the outer)
    ForEach( data, [offset](auto &arg) mutable { 
        std::cout << "doing " << arg;
        arg = arg + offset;
        offset += 10;
        std::cout << ": " << arg << " (offset increases: " << offset << ") " << std::endl;
     } );
     std::cout << " offset is still 10: " << offset << std::endl;
 
    // Capture offset by reference
    ForEach( data, [&offset](auto &arg) { 
        std::cout << "doing " << arg;
        arg = arg + offset;
        offset += 10;
        std::cout << ": " << arg << " (offset increases: " << offset << ") " << std::endl;
     } );
     std::cout << "outer offset is now 60: " << offset << std::endl;

}

int main()
{
    // define and invoke a lambda
    []() {
       std::cout << "Lambda invoked via ()" << std::endl; 
    }();  // invoke via operator()

    // invoke lambda via name
    auto myFn = []() {
       std::cout << "Lambda invoked via name" << std::endl; 
    };
    myFn();     // invoke

    // No need to specify return type
    auto test1 = []( int x, int y) { 
       return x+y;
    }(6,7);
    std::cout << "Lambda single return type, returned "<< test1 << std::endl;

    // No need to specify return type
    auto test2 = []( int x, int y)->int{ // compile error if no ->int
        if( x > 5 )
            return x+y+2.0f;    // force to int by return type above
        return x+y;
    }(6,7);
    std::cout << "Lambda two return types, returned "<< test2 << std::endl;

    TestCaptureList();

    return 0;
}