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

set(ENV{IPP_ROOT} "/home/tcaswell/intel/ipp/5.3.4.080/em64t/")
find_package(IPP REQUIRED)
set(ENV{FREEIMAGE_ROOT} "/home/tcaswell/local_installs/usr/")
find_package(FreeImage REQUIRED)


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


add_subdirectory(apps)
add_subdirectory(testing)