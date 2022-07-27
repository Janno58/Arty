cmake_minimum_required(VERSION 3.8)
project(Arty)

include_directories("src/")

# SFML
find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)

add_subdirectory(src)
