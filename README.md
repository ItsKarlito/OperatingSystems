# COEN 346 - Laboratory Projects
A repository for COEN 346's laboratory projects

## Contributors
- Jose Ricardo Monegro Quezada 
- Karl Noory 
- Yevhen Haydar 

## Building
- At the root of the repo, run the build script using the command: `./build.sh`

The script will create a **build** directory and it will call cmake followed by make within it. This will result in having all generated project executables located in **build/bin** directory.
    
## Adding a project
This way we can add new lab projects by just creating a folder in projects and adding a new entry in **projects/CMakeLists.txt**. 
For example, let's say you'd like to add a project called *test*. Here would be the steps required to do so:
- Create a folder called **projects/test/**
- Write your code into this folder. Make sure you're using `*.cpp`/`*.c` for source files, and `*.hpp`/`.h` for the header files
- Add an entry to **projects/CMakeLists.txt** with the following line:
    ```cmake
    add_project(test)
    ```
That's it!, you can now build the project like usual. You'll find an executable called **test** on **build/bin**.

# TODO: General
- [x] Add generic build system for projects

## ~~TODO: merge_sort_multithreading~~
- [x] Add multithreading to merge sort
- [x] Add file i/o support

## ~~TODO: fair_share_process_scheduling~~
- [x] Parser class
- [x] Writer class
- [x] Context switching class

## TODO: virtual_memory_manager
### Memory Manager (Jose)
- Needs to have a middle-man buffer protected by mutexes to allow for multiprocess usage
- Needs to have a thread-safe API:
    - store: Store a memory address into either memory. If there's no more space, it goes to disk
    - release: Deletes page from memory or disk
    - lookup: 
        - If id is in memory, return value from memory
        - If id is in disk, swap page from disk with page from memory that has the smallest "last_access" value. If there's a free page in memory, dont worry about swapping and just put it in there
        - If id doesn't exist, return -1

### Process Manager (Yevhen & Karl)
- Implement a FIFO scheduler
- If a process comes in during excecution, and no cores are available, move that process to a waiting queue
- Don't switch to another process until current one is done excecuting
- Timings must be calculated in milliseconds
- The number of processes that can be excecuted at the same time is equal to the numbers of cores available (specified in input file)
- Processes must pick a random amount of time that it's going to wait for, and once the timer runs out, pick the next available command from command buffer (which is a mutext protected circular filo queue)

### ~~Writer (Karl)~~
- ~~Writer object that writes to output file~~
