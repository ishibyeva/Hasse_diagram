cmake_minimum_required(VERSION 3.4)

project(main LANGUAGES CXX)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O3")

add_subdirectory(src)

# add_executable(main src/main.cpp)	
# target_link_libraries(main enum_libs)

# pybind11 binding part 
add_subdirectory(pybind11)
pybind11_add_module(face_enumeration face_enum_wrapper.cpp)
target_link_libraries(face_enumeration PRIVATE enum_libs)