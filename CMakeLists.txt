cmake_minimum_required (VERSION 2.8) 
project(TRACKING)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(COMMON_SRC_PATH ${PROJECT_SOURCE_DIR}/common_src/cxx)



SET(COMMON_SRC
 ${COMMON_SRC_PATH}/gofr_free.cpp
 ${COMMON_SRC_PATH}/hash_box.cpp
 ${COMMON_SRC_PATH}/hash_case.cpp
 ${COMMON_SRC_PATH}/hash_shelf.cpp
 ${COMMON_SRC_PATH}/particle_base.cpp
 ${COMMON_SRC_PATH}/function_cascades.cpp
 ${COMMON_SRC_PATH}/triple.cpp
 ${COMMON_SRC_PATH}/pair.cpp
 ${COMMON_SRC_PATH}/data_map.cpp
 ${COMMON_SRC_PATH}/enum_utils.cpp
 ${COMMON_SRC_PATH}/filter_basic.cpp
 ${COMMON_SRC_PATH}/master_box_t.cpp
 )

SET(COMMON_TRACK_SRC
  ${COMMON_SRC_PATH}/particle_track.cpp
  ${COMMON_SRC_PATH}/track_box.cpp
  ${COMMON_SRC_PATH}/track_list.cpp
  ${COMMON_SRC_PATH}/track_shelf.cpp 
  ${COMMON_SRC_PATH}/hash_track_funs.cpp 
  )

SET(UTIL_SRC
  ${COMMON_SRC_PATH}/array.cpp
  ${COMMON_SRC_PATH}/cell.cpp
  ${COMMON_SRC_PATH}/coarse_grain_array.cpp
  ${COMMON_SRC_PATH}/counted_vector.cpp
  ${COMMON_SRC_PATH}/histogram2d.cpp
  ${COMMON_SRC_PATH}/histogram.cpp
)

SET(CORR_SRC
  ${COMMON_SRC_PATH}/corr_gofr.cpp
  ${COMMON_SRC_PATH}/corr_goftheta.cpp
)
SET(MATLAB_WRAPPER_SRC

)

SET(OTHER_WRAPPER_SRC
  ${COMMON_SRC_PATH}/wrapper_i_file.cpp
  ${COMMON_SRC_PATH}/wrapper_i_ning.cpp
  ${COMMON_SRC_PATH}/wrapper_i_ning_hd.cpp
  ${COMMON_SRC_PATH}/wrapper_o_file.cpp
  )


SET(HDF_SRC
  ${COMMON_SRC_PATH}/wrapper_i_hdf.cpp
  ${COMMON_SRC_PATH}/wrapper_o_hdf.cpp
  ${COMMON_SRC_PATH}/generic_wrapper_hdf.cpp
  )


find_package(HDF5 REQUIRED)
include_directories(${HDF_INCLUDE_DIR})
include_directories(${TRACKING_SOURCE_DIR}/common_src/include)

SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -Wall ")

if( (NOT COMPILE_MEX) AND (NOT  COMPILE_IDEN ))
  set(COMPILE_BASIC 1)
endif( (NOT COMPILE_MEX)  AND (NOT  COMPILE_IDEN ))

if(COMPILE_MEX)
  MESSAGE(STATUS "making the mex files")
  set(COMPILE_BASIC 0)
  set(COMPILE_IDEN 0)
  add_subdirectory(matlab_apps)
endif(COMPILE_MEX)


if(COMPILE_BASIC)
  MESSAGE(STATUS "making basic functionality")
  add_subdirectory(apps)
endif(COMPILE_BASIC)


if(COMPILE_IDEN)
  MESSAGE(STATUS "making the identification functionality")
  add_subdirectory(iden/apps)
endif(COMPILE_IDEN)


# #SET(CMAKE_CXX_COMPILER "/usr/bin/g++") 
# SET(CMAKE_CXX_COMPILER "/usr/bin/g++-4.1") 


# find_package(Matlab)
# 

 

# 
# 

# set(MATLAB_MEXFILE_EXT mexa64)

# set(CMAKE_SHARED_LINKER_FLAGS  "-pthread  -Wl,--no-undefined")


# SET(CMAKE_C_FLAGS "-ansi -D_GNU_SOURCE -fexceptions -fPIC -fno-omit-frame-pointer -pthread ")


# set(CMAKE_SHARED_LINKER_FLAGS_PROFILE  " -pg")
# SET(CMAKE_CXX_FLAGS_PROFILE  " -pg")
# SET(CMAKE_C_FLAGS_PROFILE  " -pg")

# set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "-g")
# SET(CMAKE_CXX_FLAGS_DEBUG "-g")
# SET(CMAKE_C_FLAGS_DEBUG "-g")


# set(CMAKE_SHARED_LINKER_FLAGS_RELEASE  "-O3 -DNDEBUG")
# SET(CMAKE_CXX_FLAGS_RELEASE  "-O3  -DNDEBUG")
# SET(CMAKE_C_FLAGS_RELEASE   "-O3  -DNDEBUG")


# SET(MATLAB_DEFAULT_SRC /usr/local/matlab/extern/src/mexversion.c)


# add_subdirectory(cxx)
