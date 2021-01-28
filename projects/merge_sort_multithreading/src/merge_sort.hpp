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
    MergeSort(){}

    void sort(Type *arr, int start, int end)
    {
        if(start >= end)
            return;

        int m = start + (end - start)/2;
        sort(arr, start, m);
        sort(arr, m+1, end);
        
        merge(arr, start, m, m+1, end);
    }

private:
    void merge(
        Type *arr,
        int start_l, int end_l, 
        int start_r, int end_r
    )
    {

        int i = start_l;

        int i_l = 0;
        int i_r = 0;

        int size_l = end_l - start_l + 1;
        int size_r = end_r - start_r + 1;

        int arr_l[size_l] = {0};
        int arr_r[size_r] = {0};

        memcpy(arr_l, &arr[start_l], size_l*sizeof(int));
        memcpy(arr_r, &arr[start_r], size_r*sizeof(int));

        while(i_l < size_l && i_r < size_r)
        {
            if(arr_l[i_l] <= arr_r[i_r])
                arr[i] = arr_l[i_l++];
            else
                arr[i] = arr_r[i_r++];
            i++;
        }

        while (i_l < size_l) 
            arr[i++] = arr_l[i_l++];
    
        while (i_r < size_r)
            arr[i++] = arr_r[i_r++];
    }
};

#endif