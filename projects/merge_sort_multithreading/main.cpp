#include "merge_sort.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#define FILE_DIR "input.txt"
#define FILE_ERROR "ERROR: Could not read from file"

//argc = argument count
//argv = argument verbose
int main(int argc, char const *argv[])
{
    // open file, check if successful
    std::ifstream file;
    if(argc == 2)
        file = std::ifstream(argv[1]);
    else
        file = std::ifstream(FILE_DIR);
    
    if (!file.is_open())
    {
        std::cout << FILE_ERROR << std::endl;
        return EXIT_FAILURE;
    }

    // read file, store integers in vector, close file
    std::vector<int> integerVector;
    int value;
    while (file >> value)
    {
        integerVector.push_back(value);
    }
    file.close();

    // call threaded mergesort
    MergeSort<int> sort;

    sort.sort(integerVector.data(), integerVector.size());

    for (int i = 0; i < integerVector.size(); i++)
    { std::cout << integerVector[i] << ", "; }
    std::cout << "\n";

    return EXIT_SUCCESS;
}
