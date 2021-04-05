# TODO
## Memory Manager (Jose)
- Needs to have a middle-man buffer protected by mutexes to allow for multiprocess usage
- Needs to have a thread-safe API:
    - store: Store a memory address into either memory. If there's no more space, it goes to disk
    - release: Deletes page from memory or disk
    - lookup: 
        - If id is in memory, return value from memory
        - If id is in disk, swap page from disk with page from memory that has the smallest "last_access" value. If there's a free page in memory, dont worry about swapping and just put it in there
        - If id doesn't exist, return -1

## Process Manager (Yevhen)
- Implement a FIFO scheduler
- If a process comes in during excecution, and no cores are available, move that process to a waiting queue
- Don't switch to another process until current one is done excecuting
- Timings must be calculated in milliseconds
- The number of processes that can be excecuted at the same time is equal to the numbers of cores available (specified in input file)
- Processes must pick a random amount of time that it's going to wait for, and once the timer runs out, pick the next available command from command buffer (which is a mutext protected circular filo queue)

## Writer (Karl)