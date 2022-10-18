cmake_minimum_required(VERSION 3.8)
project(Prog LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(src "./build")
set_property(GLOBAL PROPERTY RULE_MESSAGES off)

