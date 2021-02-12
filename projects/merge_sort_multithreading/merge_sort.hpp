#ifndef MERGE_SORT_H_
#define MERGE_SORT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <thread>
#include <cstring>

#include <functional>

template <typename Type>
class MergeSort
{
private:
    typedef std::function<void(const std::string)> report_callback_t;

    report_callback_t report_callback;

    inline void report_thread_start_(std::thread &t, const std::thread::id i)
    {
        std::ostringstream ss;
        ss << "Thread " << i << " started\n";
        this->report(ss.str());
        ss.clear();
    }

    inline void report_thread_finish_(std::thread &t, const std::thread::id i)
    {
        std::ostringstream ss;
        ss << "Thread " << i << " finished ";
        this->report(ss.str());
        ss.clear();
    }

public:
    //initialize the output text file
    MergeSort(report_callback_t report_callback = nullptr) : report_callback(report_callback) {}

    void sort_main(Type *arr, size_t size)
    {
        //start the parent thread of merge sort
        if (size > 0)
        {
            std::thread parent(&MergeSort::sort, this, arr, 0, size - 1);
            std::thread::id parent_id = parent.get_id();
            report_thread_start_(parent, parent_id);
            parent.join();
            report_thread_finish_(parent, parent_id);
            print_array(arr, 0, size - 1);
        }
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

        //start the first thread which will deal with 1st half of given array
        //wait for first thread to finish before moving on
        std::thread first(&MergeSort::sort, this, arr, start, m);
        std::thread::id first_id = first.get_id();
        report_thread_start_(first, first_id);
        first.join();
        report_thread_finish_(first, first_id);
        print_array(arr, start, m);

        //start the second thread which will deal with 2nd half of given array
        //wait for second thread to finish before moving on
        std::thread second(&MergeSort::sort, this, arr, m + 1, end);
        std::thread::id second_id = second.get_id();
        report_thread_start_(second, second_id);
        second.join();
        report_thread_finish_(second, second_id);
        print_array(arr, m + 1, end);

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

    //print contents of an array between indeces start and end
    void print_array(Type *arr, int start, int end)
    {
        //iterate between indeces start and end
        std::ostringstream ss;

        for (int i = start; i <= end; i++)
        {
            ss << arr[i] << ", ";
        }
        ss << "\n";

        this->report(ss.str());
    }

    void report(const std::string msg)
    {
        if (this->report_callback == nullptr)
            return;

        this->report_callback(msg);
    }
};

#endif
