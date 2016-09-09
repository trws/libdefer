cmake_minimum_required(VERSION 2.8)

project(defer)

enable_testing()

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_library(defer defer.c)
target_link_libraries(defer pthread)

add_subdirectory(test)

install(TARGETS defer
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(DIRECTORY include/ 
        DESTINATION include
        FILES_MATCHING PATTERN "*")
