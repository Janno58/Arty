cmake_minimum_required(VERSION 3.8)
project(Arty)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use") # @TODO: one day fix all the includes

include_directories("src/")

# SFML
find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)

add_subdirectory(src)
