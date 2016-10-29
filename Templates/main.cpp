#include <iostream>
#include <cstring>
#include <map>

#include "MyClass.h"

template<typename T> T GetMax( T x, T y ) {
    return (x > y)?x:y;
}

// explicit specialisation for const char * types:
// Note: you also define it wil the type being specialised, which 
// makes it easier to distinguish from non-specialised explicit 
// instantiation, e.g.
// template<> const char *GetMax<const char*>( const char *x, const char *y ) {
template<> const char *GetMax( const char *x, const char *y ) {
    return strcmp(x,y)>0?x:y;
}

// Non-type template arguments
template <typename T, int size> void Print( T thing ) {
    std::cout << "non-type arg is " << size << std::endl;
}

void TestNonTypeTemplateArg()
{
    std::string x{"hello"};
    Print<std::string, sizeof(int)>(x);   // must pass a constant expression (compile time resolve)
}

template <typename T, int size> void DoMagic( T(&myArg)[size] )
{
    std::cout << "Size is " << size << std::endl;
}
void TestNonTypeTemplateArgAutoSizeDeduction()
{
    int myArray[] = {1,2,3,4 };
    int (&myRef)[4] = myArray; // () required to distinguish it from a declaration

    DoMagic(myRef);     // Note: not passing size explicitly, thanks to ref
}

template<typename T> T Add( T x, T y ) {
    return x + y;
}

template<typename T> T ArraySum( T *a, int s ) {
    T sum{};
    while( s > 0 && a != nullptr )
    {
        sum += *a;
        ++a;
        --s;
    }
    return sum;
}

template<typename T> T Max( T *a, int s ) {
    T biggest{};
    while( s > 0 && a != nullptr )
    {
        if( *a > biggest ) {
            biggest = *a;
        }
        ++a;
        --s;
    }
    return biggest;
}

template<typename T> std::pair<T,T> MinMax( T *a, int s ) {
    T biggest{*a};
    T smallest{*a};
    while( s > 0 && a != nullptr )
    {
        if( *a > biggest ) {
            biggest = *a;
        }
        if( *a < smallest ) {
            smallest = *a;
        }
        ++a;
        --s;
    }
    return std::make_pair(smallest, biggest);
}

// Variadic Templates
// Note: The args are split into 1st arg (T) + rest (MyParams) to
// aid access to the arguments via recursion.

// Base Case Function - required to end recursion (support no args call)
void VariadicPrint()
{
    std::cout << std::endl;
}
template<typename T, typename...MyParams>               // Template Parameter Pack
    void VariadicPrint(const T firstArg, const MyParams... args)    // Function Parameter Pack
    {
        // Access args via recursive call of the 'Base Case Function'
        std::cout << firstArg;
        if( sizeof...(args) > 0 ) {
            std::cout << ",";
        }

        VariadicPrint( args... );   // handle the rest
    }

// can now call with;
//      VariadicPrint( 1, 2.4f, "hello" );  // compiler generates version of this


// As above, but pass by reference 
// Note: using std::forward to supporting l and r values (so r-values
// are correctly moved, hence not const), as we pass the named arg 
// during recursion so r-values would be reinterpreted as l-values
// without the forward retaining the type info)
void VariadicPrintByRef()
{
    std::cout << std::endl;
}
template<typename T, typename...MyParams>                         // Template Parameter Pack
    void VariadicPrintByRef( T &&firstArg, MyParams&&... args)    // Function Parameter Pack
    {
        // Access args via recursive call of the 'Base Case Function'
        std::cout << firstArg;
        if( sizeof...(args) > 0 ) {
            std::cout << ",";
        }

        VariadicPrintByRef( std::forward<MyParams>(args)... );   // handle the rest
    }

// can now call with;
//      VariadicPrintByRef( 1, 2.4f, "hello" );  // compiler generates version of this


// Factory (course assignment)

template<typename T, typename...TArgPack> 
T* CreateObject( TArgPack&&... args )
{
    return new T{args...};  // automatically explanded
}

void TestFactory()
{
    int *p1 = CreateObject<int>(5) ;
    std::cout << *p1 << std::endl;

    std::string *s = CreateObject<std::string>() ;//Default construction
    std::cout << *s << std::endl;

    std::string *s2 = CreateObject<std::string>("Hello") ;//Default construction
    std::cout << *s2 << std::endl;

    MyClass *c = CreateObject<MyClass>( strdup("Sailor") );
    c->Dump(); 
/*
    Contact *p = CreateObject<Contact>(
    "Joey",                //Name
    987654321,             //Phone number
    "Boulevard Road, Sgr", //Address
    "joey@poash.com") ;    //Email
*/
};

// Class template
//  Note: size is a non-type templace argument
template<typename T, int size> class MyStack
{
    T m_Buffer[size];
    int m_Top{-1};
    // If defined inside declaration, then no need to re-state template
    void Push( const T&elem ) {
        m_Buffer[++m_Top] = elem;
    }
    // If defined outside, then...
    void Pop(void);
};

// ... need to add template spec
template<typename T, int size> void MyStack<T,size>::Pop()
{
    --m_Top;
}

// use via:
//     MyStack<std::string, 10> stack;
//     stack.Push( std::string{"x"} );


// Explicit Class Template specialisation
template<typename T> class ExplicitExample{  // general use
    T *m_Data;
    ExplicitExample(T *data) {
    }
};
template<> class ExplicitExample<char *>{  // explicit specialisation for char *
    char *m_Data;
    ExplicitExample(char *data) {
    }
};

// Class Template Partial Specialisation
template<typename T, int size> class PartialExample{  // general use
    T *m_Data;
    PartialExample(T *data) {
    }
    void DoSomethingWithData(){
        // does something, using data and size (which we can access as is constexpr)
        
    }
};
template<typename T> class PartialExample<T,80>{  // Partial Specialisation: For size 80
    T *m_Data;
    PartialExample(T *data) {
    }
    void DoSomethingWithData(){
        // does something, using data and size of 80 (which we can access as is constexpr)   
    }
};

// Type Alias
using TMyMap = std::map<std::string,int>;
void TakeAFnPtr( const char *(fnPtr)(int) ){  // Not so nice
    // do stuff and call fnPtr
}
// typedef const char *(*TMyFnPtr)(int);
using TMyFnPtr = const char *(*)(int);
void TakeANicerFnPtr( TMyFnPtr fnPtr ){  // Nicer
    // do stuff and call fnPtr
}

int main()
{
    std::cout << "GetMax(7,8) = " << GetMax(7,8) << std::endl;
    std::cout << "GetMax('c','a') = " << GetMax('c','a') << std::endl;

    std::cout << "Add(7,8) = " << Add(7,8) << std::endl;
    std::cout << "Add('c','a') = " << Add('c','a') << std::endl;

    int intArray[5] = {1,2,3,4,5};
    std::cout << "ArraySum(intArray, 5) = " << ArraySum(intArray, 5) << std::endl;
    std::cout << "Max(intArray, 5) = " << Max(intArray, 5) << std::endl;
    std::pair<int, int> intRes = MinMax(intArray, 5);
    std::cout << "MinMax(intArray, 5) = " << intRes.first << "," << intRes.second << std::endl;

    // explicit specialisation
    const char *b{"B"};
    const char *a{"A"};
    std::cout << "GetMax(*,*) = " << GetMax(a,b) << std::endl;

    TestNonTypeTemplateArg();

    TestNonTypeTemplateArgAutoSizeDeduction();

    VariadicPrint( 1, 2.4f, "hello" );
    VariadicPrintByRef( 1, 2.4f, "hello" );

    TestFactory();

    return 0;
}