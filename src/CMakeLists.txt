#cmake .. -DCMAKE_CXX_FLAGS=/MP -DBoost_INCLUDE_DIR=C:\Users\User\Documents\boost_1_55_0 -DBoost_LIBRARIES=C:\Users\User\Documents\boost_1_55_0\stage\lib

cmake_minimum_required(VERSION 2.6)
project(findrec)

find_package(boost COMPONENTS filesystem REQUIRED)
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")

#include_directories($(PROJECT_SOURCE_DIR))
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

add_executable(findrec
    main.cpp
    findfiles.cpp
    filematch.cpp
)
