cmake_minimum_required(VERSION 3.27.6)
project(auto_tuner)

set(CMAKE_CXX_STANDARD 23)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/static) # STATIC
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/shared) # SHARED

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)