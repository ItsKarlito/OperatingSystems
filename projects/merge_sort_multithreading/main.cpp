#include "merge_sort.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#define FILE_DIR "input.txt"
#define FILE_ERROR "ERROR: Could not read from file"

int main(int argc, char const *argv[])
{
    // open file, check if successful
    std::ifstream file(FILE_DIR);
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

    // create array from vector
    int integerArray[integerVector.size()] = {};
    for (int i = 0; i < integerVector.size(); i++)
    {
        integerArray[i] = integerVector[i];
    }
    std::vector<int>().swap(integerVector); // free up memory

    // call threaded mergesort
    MergeSort<int> sort;

    int size = sizeof(integerArray) / sizeof(int);

    sort.sort(integerArray, size);

    for (int i = 0; i < size; i++)
    {
        std::cout << integerArray[i] << ", ";
    }
    std::cout << "\n";

    return EXIT_SUCCESS;
}
