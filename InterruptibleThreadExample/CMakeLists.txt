# CMakeList.txt : CMake project for InterruptibleThreadExample, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (InterruptibleThreadExample "InterruptibleThreadExample.cpp" "InterruptibleThreadExample.h" "InterruptibleThread.cpp" "InterruptibleThread.hpp")

# TODO: Add tests and install targets if needed.
