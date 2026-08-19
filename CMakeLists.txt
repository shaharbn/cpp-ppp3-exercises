cmake_minimum_required(VERSION 3.20)

project(PPP3_Exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic-errors)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

add_subdirectory(part1_the_basics/chapter_01)