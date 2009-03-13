cmake_minimum_required (VERSION 2.6) 
project(TRACKING)

SET(CMAKE_CXX_COMPILER "/usr/bin/g++-4.1")

include(FindMatlab)

include_directories(${TRACKING_SOURCE_DIR}/include)
include_directories(${MATLAB_INCLUDE_DIR})

set(MATLAB_MEXFILE_EXT mexa64)

set(CMAKE_SHARED_LINKER_FLAGS  "-pthread  -Wl,--version-script,/usr/local/matlab/extern/lib/glnxa64/mexFunction.map -Wl,--no-undefined -O2")
SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -DMATLAB_MEX_FILE -O2")
SET(CMAKE_C_FLAGS "-ansi -D_GNU_SOURCE -fexceptions -fPIC -fno-omit-frame-pointer -pthread -DMATLAB_MEX_FILE -O2")


set(CMAKE_SHARED_LINKER_FLAGS_PROFILE  " -pg")
SET(CMAKE_CXX_FLAGS_PROFILE  " -pg")
SET(CMAKE_C_FLAGS_PROFILE  " -pg")

set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "-g")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_C_FLAGS_DEBUG "-g")


set(CMAKE_SHARED_LINKER_FLAGS_RELEASE  " -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE  "-O3  -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE   "-O3  -DNDEBUG")


SET(MATLAB_DEFAULT_SRC /usr/local/matlab/extern/src/mexversion.c)


add_subdirectory(cxx)