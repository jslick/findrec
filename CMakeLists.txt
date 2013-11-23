#cmake .. -DBoost_INCLUDE_DIR=C:\Users\User\Documents\boost_1_55_0 -DBoost_LIBRARIES=C:\Users\User\Documents\boost_1_55_0\stage\lib

cmake_minimum_required(VERSION 2.6)
project(findrec)

find_package(boost COMPONENTS filesystem REQUIRED)
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")

#include_directories($(PROJECT_SOURCE_DIR))
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(findrec
    main.cpp
	)
