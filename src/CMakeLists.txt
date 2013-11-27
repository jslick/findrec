#cmake ..\src\ -DCMAKE_CXX_FLAGS=/MP -DCMAKE_INCLUDE_PATH=C:\Users\User\Documents\boost_1_55_0 -DCMAKE_LIBRARY_PATH=C:\Users\User\Documents\lib

cmake_minimum_required(VERSION 2.6)
project(findrec)

include_directories(${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})
if(MSVC)
    message("Is MSVC")
else()
    find_library(BOOST_SYSTEM "boost_system" REQUIRED)
    find_library(BOOST_FILESYSTEM "boost_filesystem" REQUIRED)
endif()

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
if(MSVC)
    # Link to boost is automatic
else()
    target_link_libraries(findrec ${BOOST_SYSTEM} ${BOOST_FILESYSTEM})
endif()
