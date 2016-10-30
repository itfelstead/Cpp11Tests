#include <iostream>
/*
#include <experimental/filesystem> // for C++17 test

void TestCPP17()
{
    // path
    std::experimental::filesystem::path myPath{ R"(/mnt/c/tmp/here/file.txt)" };  // allows unix or windows style path
    if( myPath.has_filename() ) {
        std::cout << "file is " << myPath.filename() << std::endl;
    }
    for( const auto &part : myPath) {
        std::cout << "part: " << part << std::endl;
    }

    // directory_iterator(path) - list all files at path
    std::experimental::filesystem::path dirPath{ R"(/mnt/c/tmp/)" }; 
    std::experimental::filesystem::directory_iterator iter{ dirPath };
    std::experimental::filesystem::directory_iterator end{};
    while( iter != end ) {
        std::cout << *iter << std::endl;
        ++iter;
    } 
}
*/

#include <fstream>



void Test1()
{
    std::string filename{R"(/mnt/c/tmp/data.txt)"};

    // Write to file
    std::ofstream outfile{filename};    // mode out by default
    //outfile.open(filename);           // or could use .open(), rather than pass ctor arg
    outfile << "Test " << 1 << std::endl;
    outfile.close();                    // would close when out of scope anyway

    // Read from file
    std::ifstream infile{filename};
    if( infile.is_open() ) {    // or, 'infile.fail()==false' or '!infile' )
        std::string content;
        std::getline(infile, content);
        if( infile.good() ) {
            std::cout << "Read: " << content << std::endl;
        }
        else{
            std::cout << "Failed.." << std::endl;
        }
        
        infile.close();
    }
}

int main()
{
    Test1();

    return 1;
}