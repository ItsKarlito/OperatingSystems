# COEN 346 - Laboratory Projects
A repository for COEN 346's laboratory projects

## Contributors
- Jose Ricardo Monegro Quezada 
- Karl Noory 
- Yevhen Haydar 

## Building
- Create a directory called **build/** on the repository
- `cd` into it
- While inside the directory use the following commands to build the project
- `cmake ..` *This only has to be called once, unless a new project is added, in that case you'll have to use this again.*
- `make`

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

# TODO (General)
- [ ] Add generic build system for projects

## merge_sort_multithreading
### TODO
- [ ] Add multithreading to merge sort
