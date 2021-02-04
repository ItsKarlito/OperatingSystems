#ifndef MERGE_SORT_H_
#define MERGE_SORT_H_

#include <iostream>
#include <utility>
#include <thread>
#include <cstring>

template <typename Type>
class MergeSort
{
public:
    MergeSort() {}

    void sort(Type *arr, size_t size)
    {
        if (size > 0)
            sort(arr, 0, size - 1);
        else
            throw std::runtime_error("ERROR: Sorting size cannot be lower than 1");
    }

private:
    //Sort for specific segment of array
    void sort(Type *arr, int start, int end)
    {
        //Check if arr is divisible
        if (start >= end)
            return;

        //Divide arr into two segments, and sort each
        int m = start + (end - start) / 2;

        //DON'T FORGET TO JOIN THIS TWO THREADS NO MATTER WHAT
        sort(arr, start, m);
        sort(arr, m + 1, end);

        //Merge both segments into one. The end result will
        //be on the same location as arr
        merge(arr, start, m, m + 1, end);
    }

    //Merge two segments into arr
    void merge(
        Type *arr,
        int start_l, int end_l,
        int start_r, int end_r)
    {
        //left segment       right segment
        //[start_l ... end_l][start_r ... end_r]

        //Represents current index on arr
        int i = start_l;

        //Represent current index on the left
        //segment (i_l) and right segment (r_l)
        int i_l = 0;
        int i_r = 0;

        //Left and right segment sizes
        int size_l = end_l - start_l + 1;
        int size_r = end_r - start_r + 1;

        //Copy the contents of the left and right segments
        //into arr_l and arr_r respectively
        int arr_l[size_l] = {0};
        int arr_r[size_r] = {0};
        memcpy(arr_l, &arr[start_l], size_l * sizeof(Type));
        memcpy(arr_r, &arr[start_r], size_r * sizeof(Type));

        while (i_l < size_l && i_r < size_r)
        {
            //If current element on the left array
            //is bigger than the current element on
            //the right one, copy it on arr and increment i_l
            if (arr_l[i_l] <= arr_r[i_r])
                arr[i] = arr_l[i_l++];
            //Otherwise, do the same but with arr_r and i_r
            else
                arr[i] = arr_r[i_r++];

            //Increment i
            i++;
        }

        //Make sure there are no data left to be added
        while (i_l < size_l)
            arr[i++] = arr_l[i_l++];
        while (i_r < size_r)
            arr[i++] = arr_r[i_r++];
    }
};

#endif