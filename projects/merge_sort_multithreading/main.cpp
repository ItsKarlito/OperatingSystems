#include "merge_sort.hpp"

#include <iostream>
#include <fstream>
#include <vector>

//argc = argument count
//argv = argument verbose
int main(int argc, char const *argv[])
{
    //Process CLI arguments, open file, check if successful
    std::ifstream file("input.txt");
    if (argc == 2)
        file = std::ifstream(argv[1]);

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

    //Call threaded mergesort
    MergeSort<int> sort;

    sort.sort(integerVector.data(), integerVector.size());

    for (int i = 0; i < integerVector.size(); i++)
    {
        std::cout << integerVector[i] << ", ";
    }
    std::cout << "\n";

    return EXIT_SUCCESS;
}
