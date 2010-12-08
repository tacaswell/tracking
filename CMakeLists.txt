cmake_minimum_required (VERSION 2.8) 
project(TRACKING)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(COMMON_SRC_PATH ${PROJECT_SOURCE_DIR}/common_src/cxx)
set(EXTERNAL_SRC_PATH ${PROJECT_SOURCE_DIR}/external_src)

SET(CONFIG_SRC
  ${COMMON_SRC_PATH}/cl_parse.cpp
  ${COMMON_SRC_PATH}/read_config.cpp
  ${COMMON_SRC_PATH}/md_store.cpp
  ${COMMON_SRC_PATH}/enum_utils.cpp
  )

SET(UTIL_SRC
  ${COMMON_SRC_PATH}/array.cpp
  ${COMMON_SRC_PATH}/cell.cpp
#  ${COMMON_SRC_PATH}/coarse_grain_array.cpp
  ${COMMON_SRC_PATH}/counted_vector.cpp
)


SET(COMMON_SRC
  ${COMMON_SRC_PATH}/particle_base.cpp
  ${COMMON_SRC_PATH}/master_box_t.cpp
  ${COMMON_SRC_PATH}/wrapper_i.cpp
  ${COMMON_SRC_PATH}/md_store.cpp
  ${COMMON_SRC_PATH}/enum_utils.cpp
  )

SET(COMMON_HASH_SRC
  ${COMMON_SRC_PATH}/hash_box.cpp
  ${COMMON_SRC_PATH}/hash_case.cpp
  ${COMMON_SRC_PATH}/hash_shelf.cpp
  ${COMMON_SRC_PATH}/function_cascades.cpp
  ${UTIL_SRC}
  )

SET(COMMON_TRACK_SRC
  ${COMMON_SRC_PATH}/particle_track.cpp
  ${COMMON_SRC_PATH}/track_box.cpp
  ${COMMON_SRC_PATH}/track_list.cpp
  ${COMMON_SRC_PATH}/track_shelf.cpp 

  )

SET(HASH_TRACK_SRC
    ${COMMON_SRC_PATH}/hash_track_funs.cpp 
)

SET(UTIL_SRC_GNU
  ${COMMON_SRC_PATH}/histogram2d.cpp
  ${COMMON_SRC_PATH}/histogram.cpp
)
SET(CORR_SRC
  ${COMMON_SRC_PATH}/corr
  ${COMMON_SRC_PATH}/corr_gofr
  ${COMMON_SRC_PATH}/corr_case
  ${COMMON_SRC_PATH}/corr_goftheta
)

SET(ACCUM_SCR
  ${COMMON_SRC_PATH}/accum_sofq_ND.cpp
  ${COMMON_SRC_PATH}/accum_sofq.cpp
  ${COMMON_SRC_PATH}/accum_case.cpp
)

SET(COMMON_WRAPPER_SRC
  ${COMMON_SRC_PATH}/data_map.cpp
  ${COMMON_SRC_PATH}/filter_basic.cpp
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
  ${COMMON_SRC_PATH}/wrapper_o_hdf_group.cpp
  ${COMMON_SRC_PATH}/generic_wrapper_hdf.cpp
  ${COMMON_SRC_PATH}/attr_list_hdf.cpp
  ${COMMON_WRAPPER_SRC}
  ${COMMON_SRC_PATH}/md_store_attr_list
  )

set(SQL_CODE
  ${COMMON_SRC_PATH}/sql_handler.cpp
  )

SET(CORR_DISPLAY_SRC
  ${COMMON_SRC_PATH}/corr_display.cpp
)

SET(TRK_ACCUM_SRC
  ${COMMON_SRC_PATH}/ta_vanHove
  ${COMMON_SRC_PATH}/ta_msd
)
SET(ENV{HDF5_ROOT} "/home/tcaswell/local_installs")
SET(HDF5_USE_STATIC_LIBRARIES FALSE)
find_package(HDF5 REQUIRED CXX)
include_directories(${HDF_INCLUDE_DIR})
MESSAGE (STATUS "Found HDF5: ${HDF5_LIBRARIES}")
MESSAGE (STATUS "Found HDF5: ${HDF5_INCLUDE_DIRS}")



find_package(Boost COMPONENTS date_time REQUIRED)
MESSAGE (STATUS "${Boost_LIBRARIES}")


set(ENV{XERCESC_ROOT} "/home/tcaswell/local_installs")
find_package(XercesC REQUIRED)
include_directories(${XERCESC_INCLUDE_DIR})



set(ENV{SQLITE3_ROOT} "/home/tcaswell/local_installs")
find_package(SQLITE3 REQUIRED)
include_directories(${SQLITE3_INCLUDE_DIR})


include_directories(${TRACKING_SOURCE_DIR}/common_src/include)


find_library(dl_LIBRARY
  NAMES dl)
MESSAGE (STATUS "Found DL: ${dl_LIBRARY}")

if( (NOT MATLAB_MEX_FILE) AND (NOT  COMPILE_IDEN ) AND (NOT COMPILE_TEST))
  set(COMPILE_BASIC 1)
endif( (NOT MATLAB_MEX_FILE)  AND (NOT  COMPILE_IDEN )AND (NOT COMPILE_TEST))

if(MATLAB_MEX_FILE)
  MESSAGE(STATUS "making the mex files")
  set(COMPILE_BASIC 0)
  set(COMPILE_IDEN 0)
  set(COMPILE_TEST 0)
  add_subdirectory(matlab_apps)
endif(MATLAB_MEX_FILE)


if(COMPILE_BASIC)
  MESSAGE(STATUS "making basic functionality")
  SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -Wall -Wextra ")
  add_subdirectory(apps)
endif(COMPILE_BASIC)


if(COMPILE_IDEN)
  MESSAGE(STATUS "making the identification functionality")
  SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -Wall ")
  add_subdirectory(iden)
endif(COMPILE_IDEN)

if(COMPILE_TEST)
  SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -Wall")
  MESSAGE (STATUS "tried to set the CXX flages")
  MESSAGE (STATUS "${CMAKE_CXX_FLAGS}")

  add_subdirectory(testing)

endif(COMPILE_TEST)



#
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
