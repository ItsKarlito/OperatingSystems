#include "merge_sort.hpp"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

//argc = argument count
//argv = argument verbose
void log(const std::string& msg) {
    std::cout << msg;
}
std::ofstream out_file;
int main(int argc, char const *argv[])
{
    //Process CLI arguments, open file, check if successful
    std::string input_file = "input.txt";
    if (argc == 2)
        input_file = argv[1];
    std::ifstream file(input_file);

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not read from file" << std::endl;
        return EXIT_FAILURE;
    }

    //Read file, store integers in vector, close file
    std::vector<int> integerVector;
    int value;
    while (file >> value)
    {
        integerVector.push_back(value);
    }
    file.close();

    //Try to get the base path for the input (platform independent)
    std::string output_path = "output.txt";
    size_t slash_index = 0;
    if(
        ((slash_index = input_file.rfind('/')) != std::string::npos) || 
        ((slash_index = input_file.rfind('\\')) != std::string::npos)
    )
        output_path = input_file.substr(0, slash_index+1) + output_path;

    out_file = std::ofstream(output_path);

    //Call threaded mergesort
    MergeSort<int> sort([=](const std::string& msg){std::cout << msg; out_file << msg;});

    sort.sort_main(integerVector.data(), integerVector.size());

    return EXIT_SUCCESS;
}
