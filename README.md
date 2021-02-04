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

# TODO (General)
## merge_sort_multithreading
### TODO
- [ ] Add multithreading to merge sort
