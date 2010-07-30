set(IDEN_SRC_PATH ${PROJECT_SOURCE_DIR}/iden/cxx)
set(IDEN_INC_PATH ${PROJECT_SOURCE_DIR}/iden/include)

SET(COMMON_IDEN_SRC
  ${IDEN_SRC_PATH}/data_proc1.cpp
  ${IDEN_SRC_PATH}/image1.cpp
  ${IDEN_SRC_PATH}/kernel1.cpp
  ${IDEN_SRC_PATH}/params1.cpp
  ${IDEN_SRC_PATH}/iden.cpp
  ${IDEN_SRC_PATH}/wrapper_i_plu.cpp
  ${IDEN_SRC_PATH}/mm_md_parser
  ${COMMON_SRC_PATH}/md_store
)

SET(MD_PARSE_SRC
  ${IDEN_SRC_PATH}/mm_md_parser
  ${COMMON_SRC_PATH}/md_store
  ${COMMON_SRC_PATH}/enum_utils
  )

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

add_subdirectory(apps)
add_subdirectory(testing)