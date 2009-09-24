cmake_minimum_required (VERSION 2.6) 
project(TRACKING)

#SET(CMAKE_CXX_COMPILER "/usr/bin/g++") 
SET(CMAKE_CXX_COMPILER "/usr/bin/g++-4.1") 
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Matlab)
find_package(HDF5)


include_directories(${TRACKING_SOURCE_DIR}/include)
include_directories(${MATLAB_INCLUDE_DIR})
include_directories(${HDF_INCLUDE_DIR})

set(MATLAB_MEXFILE_EXT mexa64)

set(CMAKE_SHARED_LINKER_FLAGS  "-pthread  -Wl,--no-undefined")
SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread ")
SET(CMAKE_C_FLAGS "-ansi -D_GNU_SOURCE -fexceptions -fPIC -fno-omit-frame-pointer -pthread ")


set(CMAKE_SHARED_LINKER_FLAGS_PROFILE  " -pg")
SET(CMAKE_CXX_FLAGS_PROFILE  " -pg")
SET(CMAKE_C_FLAGS_PROFILE  " -pg")

set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "-g")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_C_FLAGS_DEBUG "-g")


set(CMAKE_SHARED_LINKER_FLAGS_RELEASE  "-O3 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE  "-O3  -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE   "-O3  -DNDEBUG")


SET(MATLAB_DEFAULT_SRC /usr/local/matlab/extern/src/mexversion.c)


add_subdirectory(cxx)