//
// Misc Small Features
//
// C++11 features represented in this file:
//
//	- non-static data member initialisers (member initialisers)
//	- initialiser lists
// 	- static_assert
// 	- raw string literal (R)
//  - unicode string literals (u8, u, U) and character types (char16_t, char32_t)
// 	- null pointer (nullptr)
//
// compile with -std=c++11, e.g.
// 		g++ main.cc -std=c++11
//
#include <iostream> 	// std::cout
#include <utility> 		// std::pair
#include <string>
#include <vector>


class Feature_NSMI; 	// non-static data member initialisers
class Feature_IL; 		// initialiser lists

// -----------------------------------------------------------------------------
// Feature: non-static data member initialisers
// 	non-static member variables can be initialised in-class at point of declaration.
//  pre-c++11 could initialise only static const ints/enums.
//  excludes; extern, register
// Why?
// 		Saves having to repeat initialisers in all CTORs. 	
//
class Feature_NSMI
{
	public:
		
	Feature_NSMI( )
	{
		// m_Data is 123 - feature: member initialiser
	}
	
	Feature_NSMI( int i ) : m_Data(i)
	{
		// m_Data is i
	}
	
	private:
		
	int m_Data = 123; 		// feature: member initialiser
};

// -----------------------------------------------------------------------------
// Feature: initialiser lists (not member initialiser list)
// 	allows initialisation from a {arg1, arg2,...} init list.
//  std::initializer_list is the type used to represent the init list.
// Why?
//
class Feature_IL
{
	public:
		
	Feature_IL( )
	{
		// NOOP
	}
	
	Feature_IL( int i, int j, int k ) : m_Data{i},  			// feature: initialiser lists
								m_VecData{ i, j, k} 	// feature: initialiser lists
	{
		// m_Data is i, m_VecData is [i, j, k]
	}
	
	std::pair<int, int> returnPair()
	{
		return { m_Data, m_ArrayData[2] }; 						// feature: initialiser lists (in return)
	}

	private:
		
	int m_Data {123}; 									// feature: initialiser lists
	std::vector<int> m_VecData { 1,2,3 }; 				// feature: initialiser lists
	int m_ArrayData[3] { 10, 11, 12 };
};


// -----------------------------------------------------------------------------
// Feature: static assertion (compile time)
// 	compile time assertion check (as opposed to assert()'s runtime check)
//  can put static_assert anywhere by the look of it
// Why?
//	can be used to assert static / #defined values
//
void Feature_SA_Test()
{
	static_assert( sizeof( int ) >= 4, "Static Assertions: message mandatory in c++11, not c++14" );
};


// -----------------------------------------------------------------------------
// Feature: raw string literal (R)
// 	You can specify a raw string using R"()"
//  prefix(optional) R "delimiter( raw_characters )delimiter"	
// Why?
//	prevents escape characters from being processed.
//
void Feature_RSL_Test()
{
	std::string normal{ "line 1\nline 2\n" };
	std::string raw{ R"(line 1\nline 2\n)" };
	std::cout << "Raw String Literals: normal = " << normal << ", raw = " << raw << std::endl;
}

// -----------------------------------------------------------------------------
// Feature: unicode string literals (u8, u, U) and character types (char16_t, char32_t)
//  literal operator u"": u"utf-16 string" for use with char16_t
//    escape sequence \u: requires 4 digits
//  literal operator U"": U"utf-32 string" for use with char32_t 
// 	  escape sequence \U: requires 8 digit)
//  (note: L"", for wide literal string, still supported)
// Why?
//	improve support of unicode
//
void Feature_USL_Test()
{
	// Shooting star:	
	// UTF8: 0xF0 0x9F 0x8C 0xA0 (f09f8ca0)
	// UTF 16: 0xD83C 0xDF20 (d83cdf20)
	// UTF-32: 0x0001F320 (1f320)
	// 
	const char *utf8 = u8"\xF0\x9F\x8C\xA0";
	const char16_t *utf16 = u"\uD83C\uDF20";
	const char32_t *utf32 = U"\U0001F320";
	// unfortunately can't cout them
}

// -----------------------------------------------------------------------------
// Feature: null pointer (nullptr)
//  new type to represent null pointers.
// Why?
//	safer when overloading (NULL is #define'd as int 0 usually, so clash with int)
//
class Feature_NP_Test
{
	public: 
	
	void TestFunction( char const *p )
	{
		std::cout << "nullptr: TestFunction( pointer ) called" << std::endl;
	}
	
	void TestFunction( int i )
	{
		std::cout << "nullptr: TestFunction( int ) called" << std::endl;
	}
};


// -----------------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------------
int main( )
{
	// non-static data member initialisers
	Feature_NSMI with{ 456 };
	Feature_NSMI without;
	
	// initialiser lists
	Feature_IL il{ 100, 200, 300};
	std::pair<int, int> ilPair = il.returnPair();
	std::cout << "Initialiser lists: " << ilPair.first << ", " << ilPair.second << std::endl;
	
	// static assert
	// N/A we don't have to call as it is a compile time check
	// Feature_SA_Test();
	
	// raw string literal (R)
	Feature_RSL_Test();
	
	// unicode string literal (u, U)
	Feature_USL_Test();
	
	// nullptr
	Feature_NP_Test npTest;
	// npTest.TestFunction( NULL );   // "call of overloaded ‘TestFunction(NULL)’ is ambiguous"
	npTest.TestFunction( nullptr );
	
	return 0;
}