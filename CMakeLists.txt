cmake_minimum_required(VERSION 3.10)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#thrid_party
add_subdirectory(third_party)

#source
add_subdirectory(source)