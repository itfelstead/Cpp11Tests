#include <iostream>
#include <string>
#include <cstring>

std::string ToUpper(std::string &str )
{
    std::string converted;
    
    for( auto c : str )
    {
        converted += tolower(c);
    }

    return converted;
}

std::string ToLower(std::string &str )
{
    std::string converted;

    for( auto c : str )
    {
        converted += toupper(c);
    }

    return converted;
}

int main()
{
    std::string input;
    std::cin >> input;

    std::cout << "Converting " << input << " to uppercase: " << ToUpper(input) << std::endl;
    std::cout << "Converting " << input << " to lowercase: " << ToLower(input) << std::endl;


    return 0;
}