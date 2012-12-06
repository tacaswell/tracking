cmake_minimum_required (VERSION 2.8) 
project(TRACKING)

function    (cm_export_file FILE DEST)
  if    (NOT TARGET export-files)
    add_custom_target(export-files ALL COMMENT "Exporting files into build tree")
  endif (NOT TARGET export-files)
  get_filename_component(FILENAME "${FILE}" NAME)
  add_custom_command(TARGET export-files COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${DEST}/${FILENAME}")
  install(FILES "${FILE}" DESTINATION "${DEST}")
endfunction (cm_export_file)
# taken from 
# http://stackoverflow.com/questions/697560/how-to-copy-directory-from-source-tree-to-binary-tree/3888898#3888898



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
  #  ${COMMON_SRC_PATH}/coarse_grain_array.cpp
  ${COMMON_SRC_PATH}/counted_vector.cpp
  #${COMMON_SRC_PATH}/histogram2d.cpp
  ${COMMON_SRC_PATH}/histogram.cpp
)

SET(COMMON_WRAPPER_SRC
  ${COMMON_SRC_PATH}/enum_utils.cpp
  ${COMMON_SRC_PATH}/data_map.cpp
  ${COMMON_SRC_PATH}/wrapper_i.cpp
  ${COMMON_SRC_PATH}/wrapper_i_generic.cpp
  ${COMMON_SRC_PATH}/md_store.cpp
  )



SET(COMMON_SRC
  ${COMMON_SRC_PATH}/particle_base.cpp
  ${COMMON_SRC_PATH}/master_box_t.cpp
  ${COMMON_WRAPPER_SRC}
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


SET(CORR_SRC
  ${COMMON_SRC_PATH}/corr
  ${COMMON_SRC_PATH}/corr_gofr
  ${COMMON_SRC_PATH}/corr_theta_2pt
  ${COMMON_SRC_PATH}/corr_case
  ${COMMON_SRC_PATH}/corr_goftheta
)

SET(CORR_TRACK_SRC
  ${CORR_SRC}
  ${COMMON_SRC_PATH}/corr_pair_diffusion
)

SET(ACCUM_SCR
  ${COMMON_SRC_PATH}/accum_sofq_ND.cpp
  ${COMMON_SRC_PATH}/accum_sofq.cpp
 )


SET(OTHER_WRAPPER_SRC
  ${COMMON_WRAPPER_SRC}
  ${COMMON_SRC_PATH}/wrapper_i_file.cpp
  ${COMMON_SRC_PATH}/wrapper_i_ning.cpp
  ${COMMON_SRC_PATH}/wrapper_i_ning_hd.cpp
  ${COMMON_SRC_PATH}/wrapper_o_file.cpp
  ${COMMON_SRC_PATH}/wrapper_i_jb_binary.cpp
  )


SET(HDF_SRC
  ${COMMON_WRAPPER_SRC}
  ${COMMON_SRC_PATH}/wrapper_i_hdf.cpp
  ${COMMON_SRC_PATH}/wrapper_i_generic.cpp
  ${COMMON_SRC_PATH}/wrapper_o_hdf.cpp
  ${COMMON_SRC_PATH}/wrapper_o_hdf_group.cpp
  ${COMMON_SRC_PATH}/generic_wrapper_hdf.cpp
  ${COMMON_SRC_PATH}/attr_list_hdf.cpp
  ${COMMON_SRC_PATH}/md_store_attr_list
  ${COMMON_SRC_PATH}/filter_classes.cpp
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
  ${COMMON_SRC_PATH}/ta_disp
  ${COMMON_SRC_PATH}/ta_theta_rlim
  ${COMMON_SRC_PATH}/accum_chi4_self
)
SET(ENV{HDF5_ROOT} "/home/tcaswell/local_installs")
SET(HDF5_USE_STATIC_LIBRARIES FALSE)
find_package(HDF5 REQUIRED CXX)

MESSAGE (STATUS "Found HDF5: ${HDF5_LIBRARIES}")
MESSAGE (STATUS "Found HDF5: ${HDF5_INCLUDE_DIRS}")
include_directories(${HDF5_INCLUDE_DIRS})


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


if(COMPILE_IDEN)
  MESSAGE(STATUS "making the identification functionality")
  SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -Wall ")

  set(ENV{IPP_ROOT} "/opt/intel/ipp/")
  find_package(IPP REQUIRED)
  set(ENV{FREEIMAGE_ROOT} "/home/tcaswell/local_installs/usr/")
  find_package(FreeImage REQUIRED)
  set(IDEN_SRC_PATH ${PROJECT_SOURCE_DIR}/iden/cxx)
  set(IDEN_INC_PATH ${PROJECT_SOURCE_DIR}/iden/include)

  SET(COMMON_IDEN_SRC
    ${IDEN_SRC_PATH}/data_proc1.cpp
    ${IDEN_SRC_PATH}/image1.cpp
    ${IDEN_SRC_PATH}/kernel1.cpp
    ${IDEN_SRC_PATH}/iden.cpp
    ${IDEN_SRC_PATH}/wrapper_i_plu.cpp
    )

  SET(MD_PARSE_SRC
    ${IDEN_SRC_PATH}/mm_md_parser
    ${IDEN_SRC_PATH}/md_parser_dummy
    ${COMMON_SRC_PATH}/md_store
    ${COMMON_SRC_PATH}/enum_utils
    )

  SET(IMG_WRAPPER_SRC
    ${IDEN_SRC_PATH}/image_series
    ${IDEN_SRC_PATH}/image_stack
    )


  MESSAGE (STATUS "IPP_INCLUDE : ${IPP_INCLUDE_DIRS}")
  MESSAGE (STATUS "FreeImage_INCLUDE : ${FreeImage_INCLUDE_DIR}")

  include_directories(${IPP_INCLUDE_DIRS})
  include_directories(${FreeImage_INCLUDE_DIR})
  include_directories(${IDEN_INC_PATH})
  #set(LIBS ${LIBS} ${IPPI_LIB})
  #set(LIBS ${LIBS} ${IPPS_LIB})
  #set(LIBS ${LIBS} ${IPPV_LIB})
  #set(LIBS ${LIBS} ${IPPC_LIB})
  #

  # all the infrastructure that 
  add_library(ciden2D SHARED
    ${COMMON_IDEN_SRC}
    ${MD_PARSE_SRC}
    ${IMG_WRAPPER_SRC}
    )
  set_target_properties(ciden2D PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
  target_link_libraries(ciden2D
    ${IPP_LIBRARIES} 
    ${HDF5_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${XERCESC_LIBRARY}
    ${Boost_LIBRARIES}
    )

  

  add_subdirectory(iden)
endif(COMPILE_IDEN)



if(COMPILE_BASIC)
  MESSAGE(STATUS "making basic functionality")
  SET(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -Wall -Wextra ")
  add_subdirectory(apps)
  
  # all the infrastructure that 
  add_library(ctracking2D SHARED
    ${COMMON_SRC}
    ${COMMON_HASH_SRC}
    ${COMMON_TRACK_SRC}
    ${HASH_TRACK_SRC}
    ${HDF_SRC}
    ${CORR_TRACK_SRC}
    ${CONFIG_SRC}
    ${SQL_CODE}
    ${TRK_ACCUM_SRC}
    ${UTIL_SRC}
    )
  set_target_properties(ctracking2D PROPERTIES COMPILE_DEFINITIONS  "PTYPE=1;DIM_COUNT=2")
  target_link_libraries(ctracking2D
    ${HDF5_LIBRARIES} 
    ${XERCESC_LIBRARY} 
    ${SQLITE3_LIBRARY}
    )


endif(COMPILE_BASIC)

if(COMPILE_BASIC OR COMPILE_IDEN)
  add_library(cstatic2D SHARED
    ${COMMON_SRC} 
    ${UTIL_SRC} 
    ${HDF_SRC} 
    ${CORR_SRC}
    ${COMMON_HASH_SRC} 
    ${COMMON_WRAPPER_SRC} 
    ${CONFIG_SRC}
    ${SQL_CODE}
    )
  set_target_properties(cstatic2D PROPERTIES COMPILE_DEFINITIONS  "PTYPE=0;DIM_COUNT=2")
  target_link_libraries(cstatic2D
    ${HDF5_LIBRARIES} 
    ${XERCESC_LIBRARY} 
    ${SQLITE3_LIBRARY}
    )

  
endif(COMPILE_BASIC OR COMPILE_IDEN)

FIND_PACKAGE(SWIG)
IF(SWIG_FOUND)
  add_subdirectory(script_src/swig)
  MESSAGE(STATUS "${SWIG_EXECUTABLE}")
ELSE(SWIG_FOUND)
  MESSAGE(STATUS "swig not found")
ENDIF(SWIG_FOUND)



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
