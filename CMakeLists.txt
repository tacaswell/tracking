cmake_minimum_required (VERSION 2.6) 
project(TRACKING)

SET(CMAKE_CXX_COMPILER "/usr/bin/g++-4.2")

include(FindMatlab)

include_directories(${MATLAB_INCLUDE_DIR})

set(MATLAB_MEXFILE_EXT mexa64)

set(CMAKE_SHARED_LINKER_FLAGS  "-pthread  -Wl,--version-script,/usr/local/matlab/extern/lib/glnxa64/mexFunction.map -Wl,--no-undefined")

SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -DMATLAB_MEX_FILE")

SET(CMAKE_C_FLAGS "-ansi -D_GNU_SOURCE -fexceptions -fPIC -fno-omit-frame-pointer -pthread -DMATLAB_MEX_FILE")

SET(MATLAB_DEFAULT_SRC /usr/local/matlab/extern/src/mexversion.c)


add_subdirectory(cxx)