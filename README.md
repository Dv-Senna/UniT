# UniT
A C++ library to handle units more easily

## Usage (CMake)

Include UniT as a git submodule in your project, and add it as a subdirectory
in your `CMakeLists.txt` :

```cmake
add_subdirectory(path/to/UniT)
```

Then, link `UniT::UniT` target :

```cmake
target_link_libraries(my_target PRIVATE UniT::UniT)
```

The only header file you will ever need to include is `UniT/UniT.hpp`.
