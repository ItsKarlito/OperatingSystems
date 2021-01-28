#include "merge_sort.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    MergeSort<int> sort;
    int test[] = {2,34,45,2,8,7,5,98,6};

    int size = sizeof(test)/sizeof(int);
    sort.sort(test, 0, size-1);

    for(int i = 0; i < size; i++)
    {
        std::cout << test[i] << ", ";
    }
    std::cout << "\n";

    return 0;
}
